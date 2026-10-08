# 62_StrictAliasing

Demonstrates a strict-aliasing violation (int* and float* to the same memory) and the memcpy fix.

## Files
- `01_strictAliasingViolation.c` - violateAliasing() writes through int* then float*; noinline so the compiler's aliasing assumption is what is tested; safe memcpy reinterpretation

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_strictAliasingViolation.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/62_StrictAliasing/` (git-ignored).

## Key concepts / interview angles
- An object may be accessed only through its own type, a compatible type, or `char`/`unsigned char`.
- At -O2 the compiler may return a stale cached value; the divergence does not appear at -O0.
- Fix: `memcpy` (or a union in C), or compile with `-fno-strict-aliasing` (kernel does).
- Related: `restrict`.

## Gotchas
- The violation is intentional UB; do not "fix" `violateAliasing`.
- Compare `gcc -O0` vs `gcc -O2` to see the difference; the repo Makefile builds without -O2 so it does not show by default.

## Related
- `../57_UndefinedBehaviorCatalog`
- `../23_RestrictQualifier`
- `../59_MemoryAlignmentAndPadding`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
