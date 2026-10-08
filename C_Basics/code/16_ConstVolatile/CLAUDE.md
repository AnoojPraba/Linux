# 16_ConstVolatile

const and volatile: pointer constness cheat sheet and the embedded memory-mapped register idiom; a staple of embedded interviews.

## Files
- `01_constPointers.c` - pointer to const vs const pointer
- `02_volatile.c` - volatile forces a re-read; used for hardware registers, signal handlers
- `03_deepVsShallowConstCheatSheet.c` - the four const-pointer declaration forms, read right to left
- `04_memoryMappedRegisterAccess.c` - simulated register block (static array stands in for a physical address) accessed through volatile pointers
- `NOTES.md` - const-pointer table and memory-mapped register idiom

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_constPointers.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/16_ConstVolatile/` (git-ignored).

## Key concepts / interview angles
- Read declarations right to left: `const int *p` (pointee const) vs `int * const p` (pointer const).
- `volatile` stops the compiler caching or eliding accesses; it does NOT give atomicity or ordering between threads.
- Real register access: `volatile uint32_t *reg = (volatile uint32_t *)ADDR;`.
- `const volatile` is valid (read-only status register).

## Gotchas
- `04_memoryMappedRegisterAccess.c` uses a static array, not real hardware addresses; do not point it at real physical addresses.

## Related
- `../80_SignalSafetyAndThreadLocal` - volatile sig_atomic_t
- `../27_BitFields`
- `../../../OS/code/20_Atomics` - why volatile is not atomic
- `../../../OS/code/42_HardwareBuses`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
