# 49_SystemVIPC

System V IPC: shared memory segment, message queue and semaphore set, each created with a fixed key and removed afterwards.

## Files
- `01_sharedMemorySegment.c` - shmget/shmat with key 0x1234, shmctl(IPC_RMID) cleanup
- `02_messageQueue.c` - msgget key 0x5678; typed messages, receiver can select by msgType
- `03_semaphoreSet.c` - semget key 0x9abc; semctl SETVAL; semop on a one-element set

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_sharedMemorySegment.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/49_SystemVIPC/` (git-ignored).

## Key concepts / interview angles
- SysV objects are kernel-persistent and identified by integer keys (`ftok`), not file descriptors: leaks survive process exit (inspect with `ipcs`, remove with `ipcrm`).
- Typed messages allow selective receive, unlike a pipe's byte stream.
- SysV semaphores operate on sets and support SEM_UNDO; POSIX semaphores are simpler.
- POSIX alternatives (shm_open, mq_open, sem_open) are generally preferred in new code.

## Gotchas
- Uses fixed keys 0x1234, 0x5678, 0x9abc: if a run is killed midway, remove leftovers with `ipcs` / `ipcrm` or later runs may fail or attach to stale objects.

## Related
- `../48_IPC`
- `../50_NamedPipesAndPosixQueues`
- `../35_MmapFile`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
