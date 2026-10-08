# 67_EpollInDepth

epoll level- vs edge-triggered behaviour and a single-threaded edge-triggered echo server with a built-in test client, with a senior Q&A.

## Files
- `01_levelVsEdgeTriggered.c` - same pipe data reported repeatedly (level) vs once (EPOLLET) until drained
- `02_epollEchoServer.c` - non-blocking EPOLLET echo server on an ephemeral loopback port; test client threads included
- `NOTES.md` - why epoll, level vs edge, server structure, scaling across threads, io_uring vs epoll, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_levelVsEdgeTriggered.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/67_EpollInDepth/` (git-ignored).

## Key concepts / interview angles
- EPOLLET rules: all fds non-blocking, accept in a loop until EAGAIN, read/write until EAGAIN, otherwise you lose events.
- Level-triggered is the safe default; edge-triggered reduces wakeups but is easy to get wrong.
- EPOLLONESHOT/EPOLLEXCLUSIVE for multi-threaded loops and thundering herd; one epoll fd per thread plus SO_REUSEPORT scales.
- epoll signals readiness, io_uring signals completion.

## Gotchas
- Server binds to port 0 on 127.0.0.1 (kernel-chosen ephemeral port, printed at start) and drives itself with client threads, so no second terminal is needed.

## Related
- `../54_IOMultiplexing`
- `../73_ConcurrentTcpServers`
- `../69_TcpDeepDive`
- `../55_ZeroCopyIOAndIoUring`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
