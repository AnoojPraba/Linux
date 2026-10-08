# 50_NamedPipesAndPosixQueues

Named pipes (FIFOs) and POSIX message queues, with an IPC mechanism comparison table.

## Files
- `01_namedPipeFifo.c` - mkfifo /tmp/85_demo_fifo; forked writer and reader (could be unrelated processes), unlink at the end
- `02_posixMessageQueue.c` - mq_open /85_demo_mq with priorities, mq_unlink at the end
- `NOTES.md` - FIFO, POSIX message queue and a comparison table of IPC mechanisms

## Build and run
- `01_namedPipeFifo.c` builds like every other file.
- `02_posixMessageQueue.c` uses `mq_*`: glibc older than 2.34 needs `-lrt`; this machine (glibc 2.36) links without it, and the Makefile does not pass `-lrt`. If you hit undefined `mq_open`, add `-lrt`.
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_namedPipeFifo.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/50_NamedPipesAndPosixQueues/` (git-ignored).

## Key concepts / interview angles
- A FIFO has a filesystem name, so unrelated processes can use it; open blocks until both ends are opened.
- POSIX queues keep message boundaries and priorities; names start with `/`; limits in `/proc/sys/fs/mqueue/`.
- Always unlink FIFO/queue names, or they persist.
- Compare: pipe (related processes, stream), FIFO (unrelated, stream), MQ (messages), SHM (bulk, needs sync), socket (bidirectional, networked).

## Gotchas
- Creates `/tmp/85_demo_fifo` and `/85_demo_mq`; both are cleaned up at the end of a normal run, but a killed run can leave them behind (`rm /tmp/85_demo_fifo`, `mq_unlink` via a rerun).

## Related
- `../48_IPC`
- `../49_SystemVIPC`
- `../52_SocketProgramming`
- `../54_IOMultiplexing`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
