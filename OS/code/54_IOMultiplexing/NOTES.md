# I/O Multiplexing: select, poll, epoll

Demos: `01_selectMultiplePipes.c`, `02_pollMultiplePipes.c`, `03_epollServer.c`.
Deep dive on epoll (edge/level, scaling, io_uring contrast): `../67_EpollInDepth`.
Concurrency models around accept: `../73_ConcurrentTcpServers`.

## The problem
One thread must wait on MANY file descriptors (sockets, pipes, timers) and handle
whichever becomes ready, without a thread per connection and without busy-waiting.
Blocking read on one fd would starve the others.

## I/O models (Stevens)
Blocking; non-blocking (poll in a loop - wasteful); **I/O multiplexing**
(select/poll/epoll: block until ANY fd is ready, then do a non-blocking op);
signal-driven; asynchronous (kernel does the I/O and notifies on completion -
POSIX AIO, io_uring).

## Comparison
| | select | poll | epoll |
|---|---|---|---|
| Interface | 3 fd bitsets | array of `struct pollfd` | create/ctl/wait; interest list in kernel |
| Limit | FD_SETSIZE (1024) | none (system limit) | none |
| Per-call cost | copy + scan all fds: O(n) | copy + scan all: O(n) | O(ready events) |
| Must rebuild set each call | yes (destroyed) | no (revents field) | no |
| Portability | everywhere | POSIX | Linux only (BSD: kqueue, Windows: IOCP) |
| Triggering | level | level | level or edge |
Use select/poll for few fds or portability; epoll/kqueue for thousands.

## Key facts
- **Readiness != success:** a ready fd means the operation WOULD NOT BLOCK now; a
  `read` can still return `EAGAIN` (spurious wakeup, another thread took the data) -
  use non-blocking fds. Regular files are always "ready" (not pollable).
- Read-ready on a listening socket = a connection to `accept`; EOF shows up as
  readable with `read() == 0`; `POLLHUP/POLLERR` flags report hangups/errors even if
  not requested. Write-ready only when the send buffer has space (use for backpressure).
- `select` quirks: the first arg is highest fd + 1; fd sets are modified in place; timeout
  struct may be modified on Linux; `FD_SETSIZE` overflow is UB (memory corruption).
- Timeouts: pass the nearest timer deadline as the wait timeout (or use `timerfd`);
  signals: `pselect/ppoll/epoll_pwait` close the race between unmasking and waiting.
- Handle `EINTR` by retrying; recompute remaining timeout.
- **Thundering herd / multi-thread:** see `../67_EpollInDepth` (`EPOLLEXCLUSIVE`, `SO_REUSEPORT`).
- Event loop libraries: libevent, libev, libuv (Node.js), Boost.Asio, Netty, Tokio,
  all wrap epoll/kqueue/IOCP.

## Senior interviewer Q&A
**Q: Why does epoll scale and select doesn't?**
A: `select/poll` pass and scan the full fd set on every call (O(n) each, and
`select` is capped at 1024). `epoll` keeps the interest list inside the kernel and
returns only ready fds via an internal ready list: cost scales with activity, not
with the number of connections.

**Q: What's the C10K problem and how is it solved?**
A: Serving ten thousand concurrent connections with thread-per-connection
overwhelms memory/scheduler; the answer is event-driven I/O (epoll/kqueue) with
non-blocking sockets and a small number of threads - and now C10M with kernel bypass
(DPDK), `io_uring`, thread-per-core designs.

**Q: A `poll` returns `POLLIN` but `read` returns 0 - what happened?**
A: The peer closed the connection (EOF). Remove the fd and `close` it. `read` returning
`-1/EAGAIN` instead means a spurious readiness; `-1/ECONNRESET` means RST.

**Q: Why use non-blocking sockets together with multiplexing?**
A: Readiness can be stale or partial; a blocking `read/write/accept` could stall the
whole event loop. Non-blocking calls return `EAGAIN` and let you go back to waiting.

**Q: How do you handle a slow writer/receiver in an event loop?**
A: Buffer output per connection, arm write-readiness only while data is pending,
cap the buffer (drop/close or stop reading from that peer), and use timeouts.

**Q: select vs poll vs epoll for 50 fds?**
A: Any works; at that size the difference is negligible - choose for portability
(poll) or because the project already uses an event library. Premature epoll
adds Linux-only complexity.

**Q: Can one thread block in `epoll_wait` while another modifies the interest list?**
A: Yes - `epoll_ctl` is thread-safe and takes effect for subsequent waits; but
closing an fd another thread is waiting on or reusing its number requires care
(stale events, fd reuse races).
