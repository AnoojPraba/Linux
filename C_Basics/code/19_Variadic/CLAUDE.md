# 19_Variadic

Variadic functions with stdarg.h: a mini printf and a sum function.

## Files
- `01_miniPrintf.c` - miniPrintf walks the format string with va_list, handling % specifiers via switch
- `02_sumVariadic.c` - count passed explicitly, since va_list cannot tell how many arguments exist

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_miniPrintf.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/19_Variadic/` (git-ignored).

## Key concepts / interview angles
- A variadic function must learn the argument count/types from a count, sentinel or format string; there is no type checking.
- Default promotions apply (float -> double, char/short -> int); reading the wrong type is UB.
- Always pair `va_start` with `va_end`; use `va_copy` to iterate twice.
- printf-style format mismatches are a security issue (use `__attribute__((format))`).

## Related
- `../77_PreprocessorAndC11Tricks` - variadic macros
- `../65_SecurityDemos`
- `../75_IntegerPromotionsAndConversions`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
