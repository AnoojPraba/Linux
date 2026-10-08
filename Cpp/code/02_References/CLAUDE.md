# 02_References

Lvalue references, pointers vs references, and rvalue references (the lead-in to move semantics).

## Files
- `01_references.cpp` - incrementViaReference (int&), incrementViaPointer (int*), describeRvalue (int&&) and main()

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_references.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/02_References/` (git-ignored).

## Key concepts / interview angles
- A reference is an alias: must be bound at initialisation, cannot be null or reseated.
- Pass large objects by `const T&`; use pointers when null/reseating is meaningful.
- `T&&` binds to rvalues (temporaries); this is the mechanism behind move semantics.
- Returning a reference to a local is a dangling reference.

## Related
- `../21_MoveSemantics`
- `../22_CopyElisionAndRVO`
- `../../../C_Basics/code/03_pointers`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
