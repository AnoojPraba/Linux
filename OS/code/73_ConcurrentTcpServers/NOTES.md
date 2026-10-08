# Concurrent TCP Server Models

Basic one-client-at-a-time TCP lives in `../52_SocketProgramming`; the readiness
API (`select/poll/epoll`) in `../54_IOMultiplexing` and `../67_EpollInDepth`.
This folder compares the CONCURRENCY MODELS around `accept()`.

## Socket API recap (server)
`socket()` -> `bind()` -> `listen(backlog)` -> loop `accept()` -> `read/write`
(or `recv/send`) -> `close()`. Client: `socket()` -> `connect()` -> I/O ->
`close()`. Use port 0 + `getsockname()` for a free ephemeral port in tests.
`SO_REUSEADDR` before `bind()`. Convert byte order with `htons/htonl`
(`../../../C_Basics/code/06_EndiannessAndByteOrder`). Never trust the length
a peer sends; loop on partial reads/writes (`../69_TcpDeepDive`).

## Models (and examples)
| Model | Example | Pros | Cons |
|---|---|---|---|
| Iterative | `../52_SocketProgramming` | trivial | one slow client blocks everyone |
| **Process per connection** | `01_forkPerConnection.c` | isolation, crash-safe, simple | fork cost, memory per client, IPC needed for shared state |
| **Thread per connection** | (variant of the pool) | cheap vs process, shared memory | thousands of threads = stack memory + scheduler load; races; one crash kills all |
| **Thread pool + bounded queue** | `02_threadPoolServer.c` | bounded resources, backpressure, reuse | blocking I/O can still starve the pool; head-of-line blocking |
| **Event loop (reactor)** | `../67_EpollInDepth` | 10k-1M connections, low memory | callback/state-machine style; one slow handler blocks the loop |
| **Multi-reactor (loop per core)** | `SO_REUSEPORT` + epoll per thread | scales across cores | uneven balance; no shared state |
| **Proactor / io_uring** | `../55_ZeroCopyIOAndIoUring` | async syscalls, batching | newer API, buffer lifetime rules |
| **Coroutines on an event loop** | `../../../Cpp/code/33_Cpp20Coroutines` | straight-line code, event-loop scalability | runtime/ecosystem needed |

## fork-per-connection details (`01_forkPerConnection.c`)
- Parent closes the connected fd after `fork` (else descriptors leak and the
  peer never sees EOF - the open file description is shared); child closes the
  listening fd.
- Reap zombies: `SIGCHLD` handler with `while (waitpid(-1, NULL, WNOHANG) > 0)`,
  or `SA_NOCLDWAIT`/ignore `SIGCHLD`. `accept()` may return `EINTR` - retry.
- Copy-on-write makes `fork` cheaper than it looks, but page-table copy still
  costs for big processes (use pre-fork pools).
- `fork` + threads is hazardous (only the calling thread survives).

## Thread pool details (`02_threadPoolServer.c`)
- Bounded queue guarded by mutex + two condvars (`not_empty`, `not_full`):
  producers block when full - **backpressure** instead of unbounded memory.
- Graceful shutdown: set a flag, `broadcast` the condvar, workers drain the
  queue then exit; join them.
- Always `while (cond) wait`, never `if` (spurious wakeups).
- Size the pool: CPU-bound ~ cores; I/O-bound ~ cores x (1 + wait/compute)
  (Little's law: concurrency = throughput x latency).

## Hardening checklist
Timeouts on every socket (`SO_RCVTIMEO`, idle timers), max request size,
connection limits per IP, handle `SIGPIPE`/`EPIPE`, `EMFILE` (out of fds: raise
`RLIMIT_NOFILE`, shed load), `accept4(SOCK_NONBLOCK|SOCK_CLOEXEC)`, close-on-exec
to avoid leaking sockets to child processes, graceful shutdown/draining, drop
privileges after `bind()` of low ports.

## Senior interviewer Q&A
**Q: How would you design a server for 100k concurrent connections?**
A: Event-driven (epoll/io_uring), non-blocking sockets, thread-per-core with
`SO_REUSEPORT`, per-connection state machines, small buffers (or buffer pools),
timeouts/idle reaping, kernel tuning (`somaxconn`, fd limits, `tcp_mem`, ephemeral
ports), and offload blocking work to a pool. Memory per connection (not CPU)
is usually the first limit - budget it.

**Q: Why not thread-per-connection for 100k clients?**
A: 100k stacks (even 64 KB = 6 GB), scheduler overhead, cache pollution, and
lock contention. Fine for hundreds to low thousands of mostly-busy connections.

**Q: What is the thundering herd in `accept`?**
A: Several processes/threads blocked in `accept()` or `epoll_wait()` on the same
listener are all woken for a single connection. Modern kernels wake one for
blocking `accept`; for epoll use `EPOLLEXCLUSIVE` or `SO_REUSEPORT`.

**Q: A forked server slowly runs out of memory/pids - why?**
A: Zombies (not reaped), leaked fds in the parent, or children that never exit
(slow-loris clients with no timeouts). Check `ps` state `Z`, `ls /proc/PID/fd`,
add timeouts and `SIGCHLD` reaping.

**Q: How do you shut a server down gracefully?**
A: Stop accepting (close/shutdown the listener), signal workers, finish or time
out in-flight requests, flush responses, close connections (`shutdown(SHUT_WR)`
then read until EOF to avoid RST), then exit. Handle SIGTERM via a flag/self-pipe.

**Q: `close()` vs `shutdown()`?**
A: `close` drops this process's reference to the socket (the connection ends only
when all duplicates are closed); `shutdown(SHUT_WR/RD/RDWR)` affects the
connection itself regardless of fd copies and enables half-close (send FIN but
keep reading the reply).
