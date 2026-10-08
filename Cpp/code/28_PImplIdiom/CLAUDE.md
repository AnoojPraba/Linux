# 28_PImplIdiom

Pointer-to-implementation idiom for compile-time firewalling and ABI stability.

## Files
- `01_pimplBasic.cpp` - Widget exposes only a forward-declared Impl pointer; the Impl is defined and used privately (single file, comments mark the header/source split)
- `NOTES.md` - why use PImpl, trade-offs, C analogue (opaque pointer)

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_pimplBasic.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/28_PImplIdiom/` (git-ignored).

## Key concepts / interview angles
- Hides private members and dependencies from the header: fewer rebuilds, stable ABI.
- Use `std::unique_ptr<Impl>`; the destructor and move operations must be defined where `Impl` is complete.
- Costs: heap allocation, an extra indirection, const-correctness leaks through the pointer.
- C analogue: opaque pointer pattern.

## Related
- `../../../C_Basics/code/54_OpaquePointer`
- `../15_SmartPointers`
- `../06_FriendFunctionsAndClasses`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
