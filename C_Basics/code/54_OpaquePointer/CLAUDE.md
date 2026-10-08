# 54_OpaquePointer

Opaque pointer (incomplete type) pattern for information hiding in C, the C analogue of C++ PIMPL.

## Files
- `01_main.c` - client code that can only hold a Handle* (handle->value would not compile)
- `handle.c` - real struct definition and functions (no main(); helper TU)
- `handle.h` - forward declaration of the incomplete type plus the public API

## Build and run
- Two-TU program: `gcc -Wall -Wextra -std=gnu11 01_main.c handle.c -o /tmp/x && /tmp/x`.
- The Makefile has an explicit rule for `bin/54_OpaquePointer/01_main` and excludes `handle.c` from the generic rule.
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/54_OpaquePointer/` (git-ignored).

## Key concepts / interview angles
- Callers see only `typedef struct Handle Handle;` so layout can change without recompiling clients (ABI stability).
- Constructor/destructor functions own allocation (`handleCreate`/`handleDestroy`).
- Trade-off: heap allocation and no inlining/stack instances; mention `sizeof` unavailable to callers.
- Same goal as PIMPL in `../../../Cpp/code/28_PImplIdiom`.

## Related
- `../../../Cpp/code/28_PImplIdiom`
- `../30_MultiFile`
- `../55_VTableEmulation`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
