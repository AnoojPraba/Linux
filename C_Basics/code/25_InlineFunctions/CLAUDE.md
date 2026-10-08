# 25_InlineFunctions

inline functions vs macros and the (static) inline hint.

## Files
- `01_inline.c` - static inline square() and the cost/benefit of inlining

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_inline.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/25_InlineFunctions/` (git-ignored).

## Key concepts / interview angles
- `inline` is a hint; the compiler may ignore it, and may inline un-hinted functions at -O2.
- Use `static inline` in headers to avoid multiple-definition link errors (C99 `inline` alone needs one external definition).
- Unlike macros, inline functions evaluate arguments once and are type checked.

## Related
- `../73_GenericMacro`
- `../77_PreprocessorAndC11Tricks`
- `../17_StorageClasses`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
