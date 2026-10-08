# 64_ExternC

Calling C code from C++ with extern "C" (name mangling and linkage).

## Files
- `01_main.cpp` - C++ caller using addTwo/squareOf from a C-compiled TU
- `mathutils.c` - C implementation (no main(); same code as 30_MultiFile)
- `mathutils.h` - header with `#ifdef __cplusplus extern "C" { ... }` guards

## Build and run
- Mixed-language link; the Makefile compiles C and C++ separately, then links with g++:
- `gcc -Wall -c mathutils.c -o /tmp/mathutils.o && g++ -Wall 01_main.cpp /tmp/mathutils.o -o /tmp/x && /tmp/x`
- `make` from `..` builds `bin/64_ExternC/01_main`; `01_main.cpp` is excluded from the generic .cpp rule.
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/64_ExternC/` (git-ignored).

## Key concepts / interview angles
- C++ mangles names to support overloading; C does not, so declarations must be wrapped in `extern "C"`.
- Guard with `__cplusplus` so the same header works in both languages.
- `extern "C"` affects linkage, not language semantics (no overloading, exceptions must not cross).
- Same technique wraps C libraries for C++ projects and exposes C++ via a C ABI.

## Related
- `../30_MultiFile`
- `../../../Cpp/code/01_Namespaces`
- `../54_OpaquePointer`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
