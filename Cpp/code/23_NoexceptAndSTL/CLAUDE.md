# 23_NoexceptAndSTL

Why noexcept move operations matter: std::vector falls back to copying on reallocation if move can throw.

## Files
- `01_noexceptVectorReallocation.cpp` - type with a non-noexcept move constructor shown being copied during vector growth, versus a noexcept one
- `NOTES.md` - strong exception guarantee, move_if_noexcept behaviour

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_noexceptVectorReallocation.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/23_NoexceptAndSTL/` (git-ignored).

## Key concepts / interview angles
- `vector::push_back` offers the strong guarantee, so on reallocation it uses `std::move_if_noexcept`.
- A throwing move cannot be rolled back; copying keeps the old buffer intact.
- Mark moves, swaps and destructors `noexcept`; `noexcept(expr)` operator for conditional specs.
- A violated `noexcept` calls `std::terminate`.

## Related
- `../21_MoveSemantics`
- `../13_ExceptionHandling`
- `../18_STLContainers`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
