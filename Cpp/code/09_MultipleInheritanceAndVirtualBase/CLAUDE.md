# 09_MultipleInheritanceAndVirtualBase

The diamond problem in multiple inheritance and its fix with virtual inheritance.

## Files
- `01_diamond.cpp` - two-path diamond with duplicate base subobjects, then the virtual-inheritance version with a single shared base

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_diamond.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/09_MultipleInheritanceAndVirtualBase/` (git-ignored).

## Key concepts / interview angles
- Without `virtual` inheritance the most-derived class contains two copies of the shared base (ambiguous member access).
- With virtual bases the most-derived class constructs the virtual base directly.
- Cost: extra indirection (virtual base pointer/offset), larger objects, trickier casts.
- Prefer interface-only multiple inheritance (pure abstract bases) to avoid the problem.

## Related
- `../08_Inheritance`
- `../10_Polymorphism`
- `../31_SOLIDPrinciples`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
