# 73_GenericMacro

Conceptual notes on C11 _Generic type-generic macros compared with void*-based generic C (see 31_Generics for the code).

## Files
- `NOTES.md` - how _Generic selection works; _Generic vs void*-based generic C

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Selection happens at compile time on the controlling expression type; unselected branches must still be valid expressions only syntactically (they are not evaluated).
- Beats `void *` + size parameters for type safety (no casts), but needs one association per supported type.
- Basis of type-generic math and `printf`-like helper macros.

## Related
- `../31_Generics`
- `../77_PreprocessorAndC11Tricks`
- `../61_FunctionPointersAndCallbacks`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
