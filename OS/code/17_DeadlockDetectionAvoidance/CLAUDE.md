# 17_DeadlockDetectionAvoidance

A deliberately deadlocking AB/BA lock program and the lock-ordering fix, with the Coffman conditions in NOTES.

## Files
- `01_deadlockDemo.c` - two threads take mutexA/mutexB in opposite order; hangs by design
- `02_deadlockFixed.c` - fix 1: consistent lock ordering
- `NOTES.md` - four necessary conditions, detection, avoidance, prevention, recovery

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_deadlockDemo.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/17_DeadlockDetectionAvoidance/` (git-ignored).

## Key concepts / interview angles
- Coffman conditions: mutual exclusion, hold-and-wait, no preemption, circular wait; break any one.
- Lock ordering is the practical fix; also `pthread_mutex_trylock`/timeouts and `std::scoped_lock`.
- Detect with a wait-for graph; diagnose live with `gdb` (`thread apply all bt`) or TSan lock-order inversion reports.
- Banker's algorithm is avoidance, rarely used in practice.

## Gotchas
- `01_deadlockDemo.c` hangs on purpose; press Ctrl+C after the "acquired first lock" lines. Do not fix it.

## Related
- `../18_ResourceAllocationGraphAndBankersAlgorithm`
- `../19_StarvationLivelockAndDeadlockPrevention`
- `../09_Threads/05_deadlockAvoidance.c`
- `../../../Cpp/code/24_Concurrency/10_deadlockAndScopedLock.cpp`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
