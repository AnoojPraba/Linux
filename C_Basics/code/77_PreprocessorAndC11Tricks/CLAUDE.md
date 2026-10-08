# 77_PreprocessorAndC11Tricks

Senior-level preprocessor idioms (X-macros, stringify/paste, variadic macros) and C99/C11 features (static_assert, designated initialisers, compound literals).

## Files
- `01_xmacro.c` - one state list expanded into enum, name table and handler table
- `02_c99_c11_features.c` - compile-time checks via _Static_assert plus other C99/C11 features
- `NOTES.md` - preprocessor tricks, C99/C11 features seen in senior code, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_xmacro.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/77_PreprocessorAndC11Tricks/` (git-ignored).

## Key concepts / interview angles
- X-macros keep enums, string tables and dispatch tables in sync from one definition.
- `do { } while (0)` makes multi-statement macros safe in if/else; parenthesise arguments; avoid double evaluation.
- `#` stringify and `##` paste; `__VA_ARGS__` and `__LINE__`/`__FILE__`.
- `_Static_assert` fails the build, not the run: use for struct layout and wire-format sizes.
- Designated initialisers and compound literals for readable tables.

## Related
- `../73_GenericMacro`
- `../31_Generics`
- `../25_InlineFunctions`
- `../59_MemoryAlignmentAndPadding`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
