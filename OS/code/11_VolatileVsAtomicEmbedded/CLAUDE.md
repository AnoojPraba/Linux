# 11_VolatileVsAtomicEmbedded

Why volatile is not atomic: its correct narrow use (signal/ISR flags, MMIO) vs _Atomic for cross-thread sharing.

## Files
- `01_volatileNotAtomic.c` - volatile sig_atomic_t flag set by a SIGINT handler and polled by main (the correct narrow use)
- `02_atomicCounter.c` - _Atomic counter incremented by threads: no lost updates, defined ordering
- `NOTES.md` - volatile vs _Atomic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_volatileNotAtomic.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/11_VolatileVsAtomicEmbedded/` (git-ignored).

## Key concepts / interview angles
- `volatile` prevents the compiler from caching/eliding accesses; it gives no atomicity and no inter-thread ordering.
- Correct uses: memory-mapped registers, flags shared with a signal handler/ISR on one core.
- `_Atomic`/`stdatomic.h` give atomic read-modify-write and memory-order semantics.
- On embedded single-core, ISR-shared data often needs both `volatile` and critical sections (disable interrupts).

## Related
- `../20_Atomics`
- `../12_RaceConditionAndCriticalSection`
- `../../../C_Basics/code/16_ConstVolatile`
- `../37_LockFreeRingBuffer`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
