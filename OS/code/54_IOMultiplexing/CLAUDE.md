# 54_IOMultiplexing

select, poll and epoll over pipes, with I/O models (Stevens), a comparison table, key facts and a senior Q&A.

## Files
- `01_selectMultiplePipes.c` - select() on two pipes; only pipeB is written, so only it becomes ready
- `02_pollMultiplePipes.c` - poll() with a pollfd array (no FD_SETSIZE cap)
- `03_epollServer.c` - epoll_create1/epoll_ctl/epoll_wait over pipes: interest list kept in the kernel
- `NOTES.md` - the problem, Stevens I/O models, comparison, key facts, "Senior interviewer Q&A" (why epoll scales, C10K, POLLIN with read 0, non-blocking sockets, slow writers, select vs poll vs epoll, concurrent modification)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_selectMultiplePipes.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/54_IOMultiplexing/` (git-ignored).

## Key concepts / interview angles
- select: bitmask limited by FD_SETSIZE (1024), O(n) scan and rebuild every call; poll: no cap but still O(n) per call; epoll: O(ready) and the interest set persists in the kernel.
- Level-triggered vs edge-triggered: edge-triggered requires non-blocking fds and reading until EAGAIN.
- `POLLIN` plus `read() == 0` means EOF/peer close.
- Multiplexing readiness is not completion: compare io_uring (completion model).
- Slow receivers need per-connection output buffering and EPOLLOUT interest.

## Related
- `../67_EpollInDepth`
- `../73_ConcurrentTcpServers`
- `../51_NetworkStackBasics`
- `../55_ZeroCopyIOAndIoUring`
- `../48_IPC`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
