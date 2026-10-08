# 73_ConcurrentTcpServers

Concurrent TCP server models: fork-per-connection and a thread pool with a bounded queue, each self-driven by built-in clients on an ephemeral loopback port, with a hardening checklist and Q&A.

## Files
- `01_forkPerConnection.c` - accept then fork a child per connection; SIGCHLD reaping; built-in clients
- `02_threadPoolServer.c` - main thread accepts and queues fds; fixed worker threads pop and serve
- `NOTES.md` - socket API recap, models, fork and thread-pool details, hardening checklist, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_forkPerConnection.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/73_ConcurrentTcpServers/` (git-ignored).

## Key concepts / interview angles
- fork-per-connection: strong isolation, simple, but process cost and zombie handling (SIGCHLD).
- Thread pool with bounded queue: caps resource use and applies backpressure; shared-state bugs possible.
- Event loop (epoll) scales further but needs non-blocking code; hybrid designs use one loop per core.
- Hardening: backlog, timeouts, max connections, `SO_REUSEPORT`, closing fds in children, SIGPIPE.

## Gotchas
- Both programs bind port 0 on loopback (ephemeral, printed) and spawn their own clients, so a single run is self-contained.

## Related
- `../52_SocketProgramming`
- `../67_EpollInDepth`
- `../54_IOMultiplexing`
- `../69_TcpDeepDive`
- `../../../Cpp/code/24_Concurrency/06_threadPool.cpp`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
