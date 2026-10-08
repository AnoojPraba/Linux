# 21_AdvancedSyncPrimitives

Reader/writer locks, barriers and a from-scratch fair (starvation-free) rwlock.

## Files
- `01_rwlockDemo.c` - pthread_rwlock_t with concurrent readers and an exclusive writer
- `02_barrierDemo.c` - pthread_barrier_t phased computation: all threads finish phase 1 before phase 2
- `03_fairRwLockFromScratch.c` - fair rwlock from one mutex and two condition variables (new readers wait if a writer is waiting)
- `NOTES.md` - pthread_rwlock, pthread_barrier, from-scratch fair rwlock

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_rwlockDemo.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/21_AdvancedSyncPrimitives/` (git-ignored).

## Key concepts / interview angles
- rwlock helps only when reads dominate and critical sections are long enough; otherwise a mutex is faster.
- Reader-preference starves writers; a fair lock blocks new readers once a writer waits.
- Barrier gives phase synchronisation (BSP-style parallel loops); `PTHREAD_BARRIER_SERIAL_THREAD` picks one thread.
- Upgrade/downgrade of rwlocks is a deadlock hazard.

## Gotchas
- Demos use sleeps, so each run takes a second or more.

## Related
- `../14_SyncProblems/02_readerWriter.c`
- `../65_FutexAndSeqlock` - seqlock as a read-mostly alternative
- `../38_ConcurrentDataStructures`
- `../../../Cpp/code/24_Concurrency/07_sharedMutexReaderWriter.cpp`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
