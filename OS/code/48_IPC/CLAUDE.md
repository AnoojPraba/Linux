# 48_IPC

Core POSIX IPC between related processes and threads: pipes, anonymous shared memory, process-shared semaphores and an odd/even semaphore ping-pong.

## Files
- `01_pipeCommunication.c` - pipe() between a parent and a forked child (fd[0] read end, fd[1] write end)
- `02_sharedMemory.c` - MAP_ANONYMOUS | MAP_SHARED region visible to parent and child after fork
- `03_processSemaphore.c` - sem_t in shared mmap memory with pshared nonzero, usable across fork
- `04_oddorEven_semaphore.c` - two threads alternate printing using two semaphores (sem_odd/sem_even)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_pipeCommunication.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/48_IPC/` (git-ignored).

## Key concepts / interview angles
- Pipes are unidirectional byte streams with kernel buffering (64 KB default on Linux) and blocking semantics; close unused ends so readers see EOF.
- Shared memory is the fastest IPC but needs synchronisation (here a process-shared semaphore).
- `sem_init(&s, 1, n)` pshared must be nonzero and the sem_t must live in shared memory.
- Choose IPC by need: pipe/FIFO (stream), message queue (discrete, prioritised), shared memory (bulk), sockets (network or local, bidirectional).

## Related
- `../49_SystemVIPC`
- `../50_NamedPipesAndPosixQueues`
- `../52_SocketProgramming`
- `../09_Threads`
- `../36_CopyOnWrite`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
