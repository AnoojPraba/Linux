# 65_FutexAndSeqlock

Futex-based mutex (Drepper mutex2) and a seqlock: how fast-path user-space synchronisation meets the kernel, with a senior Q&A.

## Files
- `01_futex_mutex.c` - mutex from futex(2): states 0 unlocked, 1 locked, 2 locked with waiters; atomic fast path in user space
- `02_seqlock.c` - single-writer many-reader seqlock: odd sequence means write in progress, readers retry
- `NOTES.md` - futex, seqlock, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_futex_mutex.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/65_FutexAndSeqlock/` (git-ignored).

## Key concepts / interview angles
- Futex: kernel is entered only on contention (FUTEX_WAIT/WAKE); the uncontended lock/unlock is a single atomic op.
- State 2 ("waiters") avoids unnecessary wake syscalls on unlock.
- Seqlock: readers never block the writer, but may retry and must not follow pointers inside a torn read; good for small read-mostly data like timekeeping.
- Pthread mutexes, condvars and semaphores on Linux are built on futexes.
- Priority inversion and fairness are not handled by this simple mutex.

## Related
- `../20_Atomics`
- `../21_AdvancedSyncPrimitives`
- `../66_MemoryModelLitmusTests`
- `../37_LockFreeRingBuffer`
- `../../../C_Basics/code/80_SignalSafetyAndThreadLocal`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
