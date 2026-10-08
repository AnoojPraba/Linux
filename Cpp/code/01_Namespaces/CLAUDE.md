# 01_Namespaces

Namespace basics: nested namespaces, qualification and using declarations.

## Files
- `01_namespaces.cpp` - namespace Geometry with nested ThreeD; calls with full qualification and via `using Geometry::unitSquareArea`

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_namespaces.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/01_Namespaces/` (git-ignored).

## Key concepts / interview angles
- Namespaces prevent name collisions; nested ones group related APIs.
- Prefer using-declarations (single name) over `using namespace` in headers (pollutes every includer).
- Anonymous namespaces give internal linkage (the C++ replacement for file-scope `static`).
- Argument-dependent lookup (ADL) can find functions in an argument's namespace.

## Related
- `../02_References`
- `../../../C_Basics/code/17_StorageClasses` - C linkage equivalents
- `../../../C_Basics/code/64_ExternC`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
