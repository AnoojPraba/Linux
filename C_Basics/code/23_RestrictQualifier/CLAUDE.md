# 23_RestrictQualifier

The C99 restrict qualifier: a no-aliasing promise that lets the compiler optimise memory operations.

## Files
- `01_restrict.c` - function taking restrict dest/src pointers, explaining that the compiler may skip re-reading src after writes to dest

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_restrict.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/23_RestrictQualifier/` (git-ignored).

## Key concepts / interview angles
- `restrict` promises that, for the pointer's lifetime, the object it points to is accessed only through it (or pointers derived from it).
- Violating the promise is UB; `memcpy` has restrict params, `memmove` does not.
- The payoff is vectorisation and load/store reordering; inspect with `gcc -O2 -S`.

## Related
- `../62_StrictAliasing`
- `../63_MemmoveImplementation` - overlap handling
- `../78_BranchHintsPrefetchAndCacheLayout`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
