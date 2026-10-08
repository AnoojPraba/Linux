# 14_SyncProblems

Classic synchronisation problems solved with pthreads: bounded-buffer producer/consumer, readers-writers and dining philosophers.

## Files
- `01_boundedBufferProducerConsumer.c` - ring of BUFFER_SIZE slots guarded by a mutex with notFull/notEmpty condition variables
- `02_readerWriter.c` - many concurrent readers (activeReaders count) or one exclusive writer
- `03_diningPhilosophers.c` - five philosophers; the highest-numbered one picks up forks in the opposite order to break circular wait

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_boundedBufferProducerConsumer.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/14_SyncProblems/` (git-ignored).

## Key concepts / interview angles
- Bounded buffer: producer waits on notFull, consumer on notEmpty; always loop on the predicate.
- Readers-writers: reader-preference can starve writers; writer-preference or fair locks avoid it (see `../21_AdvancedSyncPrimitives/03_fairRwLockFromScratch.c`).
- Dining philosophers: deadlock needs all four Coffman conditions; asymmetric ordering, a waiter/semaphore limit, or try-lock break it.
- Know the semaphore versions of each solution.

## Related
- `../09_Threads`
- `../15_MutexVsSemaphoreAndMonitors`
- `../17_DeadlockDetectionAvoidance`
- `../19_StarvationLivelockAndDeadlockPrevention`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
