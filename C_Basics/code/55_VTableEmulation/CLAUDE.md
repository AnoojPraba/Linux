# 55_VTableEmulation

Emulating C++ virtual dispatch in C with a hand-rolled vtable (struct of function pointers).

## Files
- `01_shapeVtable.c` - Shape "base class" holding a vtable pointer; circle/rectangle "subclasses" supply their own function tables

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_shapeVtable.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/55_VTableEmulation/` (git-ignored).

## Key concepts / interview angles
- A C++ vtable is literally a per-class table of function pointers plus a vptr in each object.
- "Inheritance" = embedding the base struct as the first member so a derived pointer can be cast to the base.
- Polymorphic call = `obj->vtbl->area(obj)`; one extra indirection, hard for the compiler to inline.
- This is how the Linux kernel (`file_operations`) and many C libraries do OO.

## Related
- `../../../Cpp/code/10_Polymorphism`
- `../61_FunctionPointersAndCallbacks`
- `../74_ContainerOfAndIntrusiveLists`
- `../54_OpaquePointer`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
