# 31_Generics

C11 _Generic: compile-time type dispatch for type-generic macros.

## Files
- `01_genericMacro.c` - `add(a, b)` macro that selects an implementation by argument type, plus a `typeName(x)` macro

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_genericMacro.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/31_Generics/` (git-ignored).

## Key concepts / interview angles
- `_Generic` selects an expression at compile time from the controlling expression's type; there is no runtime cost.
- The controlling expression undergoes lvalue conversion, so qualifiers and arrays decay (e.g. `const int` matches `int`).
- It is how `<tgmath.h>` style APIs are built in C.

## Related
- `../73_GenericMacro`
- `../77_PreprocessorAndC11Tricks`
- `../../../Cpp/code/17_Templates` - the C++ equivalent

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
