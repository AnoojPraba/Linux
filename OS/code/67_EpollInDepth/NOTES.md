# epoll in Depth (and where io_uring differs)

## Why epoll
- `select`/`poll` pass the whole fd set to the kernel on every call and scan it
  linearly: O(n) per wait. `epoll` keeps the interest list in the kernel
  (`epoll_ctl` add/mod/del) and `epoll_wait` returns only READY fds (ready list
  fed by wakeup callbacks from each file) - O(ready), scales to 100k+ fds
  (the C10K/C10M answer). See `../54_IOMultiplexing` for select/poll.
- API: `epoll_create1`, `epoll_ctl(EPOLL_CTL_ADD|MOD|DEL)`, `epoll_wait`.
  Regular files are not pollable (always "ready") - epoll is for sockets,
  pipes, eventfd, timerfd, signalfd, inotify.

## Level vs edge triggered (`01_levelVsEdgeTriggered.c`)
- **Level (default):** reported while the condition holds. Forgiving: partial
  reads are fine, you get notified again.
- **Edge (`EPOLLET`):** reported once per state change. Fewer wakeups, but you
  MUST: use non-blocking fds, and read/accept/write until `EAGAIN`; otherwise
  leftover data stalls forever. Classic hang bug in interviews.
- **`EPOLLONESHOT`:** disarm after one event until `EPOLL_CTL_MOD` re-arms it -
  guarantees one thread handles a given fd at a time in a worker pool.
- **`EPOLLRDHUP`:** peer closed its write side (detect half-close without a read).
- **`EPOLLOUT`:** only arm while you have unsent data; a writable socket is
  almost always ready, so leaving it armed spins at 100% CPU (level mode).

## Server structure (`02_epollEchoServer.c`)
- Non-blocking listen socket; accept loop until `EAGAIN`.
- Per-connection state (read buffer, pending write buffer, parser state) kept in
  a struct pointed to by `event.data.ptr`, not just the fd.
- Partial writes: `write` returns fewer bytes or `EAGAIN` -> keep the rest in a
  queue, arm `EPOLLOUT`, resume when writable (backpressure).
- Closing an fd removes it from epoll automatically only when ALL duplicates of
  the open file description are closed (fork/dup surprise); `EPOLL_CTL_DEL`
  explicitly if unsure. Reusing an fd number while stale events are in the
  returned batch is a classic bug - handle per-event validation / generation counters.

## Scaling across threads/cores
- **One epoll per thread + `SO_REUSEPORT`:** each thread owns a listening
  socket on the same port; the kernel load-balances new connections by hash.
  No shared state, best scaling (nginx, many modern servers).
- **Shared epoll + several threads:** use `EPOLLONESHOT` (or `EPOLLEXCLUSIVE`
  on the listen fd) to avoid two threads handling one fd.
- **Thundering herd:** many processes/threads blocked in `epoll_wait` on the
  same listening fd are ALL woken for one connection; `EPOLLEXCLUSIVE` (4.5+)
  wakes one. (Plain `accept()` blocking has been herd-free since Linux 2.6.)
- **Reactor vs Proactor:** epoll is a reactor ("tell me when I can read/write,
  then I do the syscall"). io_uring/IOCP are proactors ("do the I/O and tell me
  when it completed").

## io_uring vs epoll (mechanics)
- Two lock-free ring buffers shared with the kernel via `mmap`: the
  **submission queue (SQ)** you fill with SQEs, and the **completion queue
  (CQ)** the kernel fills with CQEs. `io_uring_enter` submits/waits; with
  `SQPOLL` a kernel thread polls the SQ so submissions need no syscall.
- Completion-based: actually performs read/write/accept/send/recv/fsync/...
  asynchronously, including for regular files (epoll cannot). Batching reduces
  syscalls; registered buffers/files avoid per-op lookups and copies; fixed
  `provided buffers` for multishot recv.
- Trade-offs: newer API surface and kernel-version dependence, history of
  security issues (often disabled in sandboxes/containers), more complex error
  and lifetime handling (buffers must stay valid until completion).
  See `../55_ZeroCopyIOAndIoUring`.

## Senior interviewer Q&A
**Q: Why does epoll scale better than select/poll?**
A: `select/poll` re-send and re-scan the entire fd set per call (O(n) copy and
scan, `select` capped at FD_SETSIZE=1024). `epoll` registers interest once;
readiness is pushed onto a ready list via callbacks, so `epoll_wait` cost is
O(ready events).

**Q: Edge-triggered gotchas?**
A: Must use non-blocking sockets and loop until `EAGAIN` for read/accept;
beware starvation (one busy fd monopolizing the loop - cap work per event);
missing an edge = hung connection; combine with `EPOLLONESHOT` in threaded
designs. Level-triggered is simpler and often fast enough.

**Q: A socket is ready for read - can `read()` still block?**
A: Yes on a blocking fd: readiness is a hint (spurious wakeups, another
thread consumed the data, checksum failure drops a UDP packet after select
returns). Always non-blocking.

**Q: Why not use `EPOLLOUT` all the time?**
A: Sockets are nearly always writable; level-triggered `EPOLLOUT` makes
`epoll_wait` return immediately in a busy loop. Arm it only when you hit
`EAGAIN` with data pending, and disarm when the queue drains.

**Q: How do you handle a slow client (backpressure)?**
A: Per-connection output queue with a high-water mark; stop reading from that
client (disarm `EPOLLIN`) when the queue is full, or drop/close; never block the
loop; timeouts via `timerfd` or a timer wheel/heap integrated with the
`epoll_wait` timeout.

**Q: How would you scale a server across 32 cores?**
A: Thread-per-core with its own epoll and `SO_REUSEPORT` listeners (shared-
nothing), pin threads and IRQs/RSS queues, keep per-connection state local;
offload CPU-heavy work to a pool. Watch for uneven connection distribution
(long-lived connections) - `SO_INCOMING_CPU`, eBPF reuseport programs.

**Q: epoll on regular files?**
A: `EPERM` - files are always "ready" to the kernel's poll. For async file I/O
use `io_uring`, `libaio` (O_DIRECT only) or a thread pool (what libuv does).

**Q: What is the difference between the epoll interest list and ready list, and what happens on `fork`?**
A: Interest list = all registered fds (a red-black tree in the kernel); ready
list = those currently ready. The epoll fd is inherited by children and
shares the same instance; avoid sharing across `fork` without care.
