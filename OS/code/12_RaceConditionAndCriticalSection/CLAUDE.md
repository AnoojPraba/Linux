# 12_RaceConditionAndCriticalSection

Race conditions and critical sections demonstrated with an unprotected shared counter.

## Files
- `01_unprotectedCounterRace.c` - threads increment a shared counter without synchronisation and lose updates
- `NOTES.md` - race condition, critical section, why `counter++` is load/add/store, fixes

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_unprotectedCounterRace.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/12_RaceConditionAndCriticalSection/` (git-ignored).

## Key concepts / interview angles
- `counter++` is three steps; interleaving causes lost updates.
- Critical section requires mutual exclusion, progress and bounded waiting.
- Fixes: mutex, atomic RMW, or eliminate sharing (per-thread counters merged at the end).
- Data races are UB in C11, not merely "wrong output".

## Gotchas
- The race is intentional: results vary run to run and may sometimes look correct; do not fix it in this file (the fixed versions are in `../09_Threads/02_mutex.c` and `../11_VolatileVsAtomicEmbedded/02_atomicCounter.c`).

## Related
- `../09_Threads/02_mutex.c`
- `../11_VolatileVsAtomicEmbedded`
- `../20_Atomics`
- `../../../C_Basics/code/66_ThreadSanitizerDemo`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
