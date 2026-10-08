# 08_Structures

Structs, nested structs and unions: assignment semantics, composition and shared storage.

## Files
- `01_basicStruct.c` - struct assignment is a member-wise (shallow) copy
- `02_nestedStruct.c` - struct Rectangle of two Points, array of structs, area via pointer
- `03_union.c` - union members share storage; writing one clobbers the other

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_basicStruct.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/08_Structures/` (git-ignored).

## Key concepts / interview angles
- Struct assignment copies bytes including padding; a pointer member is copied shallowly (aliasing).
- Unions give type punning (well-defined in C99+ via the union, not in C++).
- `->` is shorthand for `(*p).`.
- sizeof(struct) may exceed the sum of members; see `../59_MemoryAlignmentAndPadding`.

## Related
- `../59_MemoryAlignmentAndPadding`
- `../27_BitFields`
- `../09_Enum`
- `../62_StrictAliasing`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
