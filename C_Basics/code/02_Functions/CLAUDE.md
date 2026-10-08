# 02_Functions

Short C trivia programs on float/double literals, macros, integer literal limits and variable scope; classic "what does this print" interview questions.

## Files
- `01_datatype.c` - float x = 0.1 compared to 0.1 (double) and 0.1f - which branch runs
- `02_datatype.c` - same comparison with 0.5, which is exactly representable
- `03_datatype.c` - sizeof(float) vs sizeof(0.1) vs sizeof(0.1f)
- `04_define.c` - #define / #undef / redefine of a macro via #ifdef
- `05_int_max.c` - int literal 2147483648 overflow (INT_MAX line commented out)
- `06_scope.c` - global x vs local shadowing x; f() reads the global even when called from g()
- `07_scope.c` - nested block scope and shadowing of local variables

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_datatype.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/02_Functions/` (git-ignored).

## Key concepts / interview angles
- An unsuffixed floating literal is `double`; `float x = 0.1` stores a rounded value, so `x == 0.1` is false but `x == 0.1f` is true.
- 0.5 is exactly representable in binary, so both comparisons succeed (contrast with 0.1).
- C scoping is lexical (static), not dynamic: a callee sees the global, not the caller's local.
- Macros are textual: `#undef` then `#define` legally redefines.

## Gotchas
- `05_int_max.c` intentionally uses an out-of-range literal; expect a compiler warning, the output is implementation-defined. Do not fix it.

## Related
- `../57_UndefinedBehaviorCatalog` - overflow and other UB
- `../17_StorageClasses` - linkage and storage duration
- `../75_IntegerPromotionsAndConversions` - literal and conversion rules

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
