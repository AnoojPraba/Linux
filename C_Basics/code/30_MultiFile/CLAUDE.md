# 30_MultiFile

Splitting a program over several translation units: header, include guard, and two-file compile/link.

## Files
- `01_main.c` - main() calling addTwo and squareOf
- `mathutils.c` - definitions (no main(); helper TU)
- `mathutils.h` - declarations with #ifndef include guard

## Build and run
- This is a two-TU program; the generic one-file rule does not apply. Makefile rule links `01_main.c` + `mathutils.c`.
- By hand: `gcc -Wall -Wextra -std=gnu11 01_main.c mathutils.c -o /tmp/x && /tmp/x`.
- `make` from `..` builds it into `C_Basics/bin/30_MultiFile/01_main` (git-ignored); `mathutils.c` is excluded from the generic rule.
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/30_MultiFile/` (git-ignored).

## Key concepts / interview angles
- Headers declare, `.c` files define; include guards (or `#pragma once`) prevent double inclusion.
- Compile each TU separately (`gcc -c`) and link; a missing definition shows up as an undefined-reference at link time.
- Use `static` to keep helpers internal.

## Related
- `../17_StorageClasses`
- `../29_CompilerPipelineWalkthrough`
- `../54_OpaquePointer`
- `../71_CMakeIntroduction`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
