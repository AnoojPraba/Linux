# 13_ClassicalSyncAlgorithms

Classical mutual-exclusion algorithms (Peterson, Dekker, Bakery) and the hardware test-and-set / compare-and-swap that replaced them.

## Files
- `01_petersonsAlgorithm.c` - two-thread flag[] + turn, built on atomic_int
- `02_dekkersAlgorithm.c` - original two-process software solution
- `03_bakeryAlgorithm.c` - Lamport bakery for N threads (ticket numbers)
- `04_hardwareTestAndSetCompareAndSwap.c` - spinlock from atomic_flag test-and-set and compare-and-swap
- `NOTES.md` - classical mutual exclusion algorithms and why they are rarely used now

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_petersonsAlgorithm.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/13_ClassicalSyncAlgorithms/` (git-ignored).

## Key concepts / interview angles
- Peterson/Dekker need sequentially consistent ordering; with plain loads/stores, modern CPUs and compilers reorder and break them (the demos use `atomic_int`).
- Bakery generalises to N threads and works without atomic RMW.
- TAS/CAS spinlocks: simple, but waste CPU, unfair, and suffer cache-line bouncing (see test-and-test-and-set, ticket/MCS locks).
- These are taught for reasoning about correctness; production code uses mutexes/atomics.

## Gotchas
- Spin loops burn CPU while waiting; on a heavily loaded machine runs may take noticeably longer.

## Related
- `../14_SyncProblems`
- `../20_Atomics`
- `../21_AdvancedSyncPrimitives`
- `../65_FutexAndSeqlock`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
