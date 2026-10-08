# 21_MoveSemantics

Move constructor and move assignment on a class owning a heap array, with std::move and rvalue references.

## Files
- `01_moveSemantics.cpp` - class with copy and move special members; traces which one runs

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_moveSemantics.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/21_MoveSemantics/` (git-ignored).

## Key concepts / interview angles
- `std::move` is just a cast to an rvalue; the move constructor does the stealing.
- Leave the moved-from object valid but unspecified (null the pointer).
- Mark move operations `noexcept` so containers use them (see `../23_NoexceptAndSTL`).
- Never `return std::move(local)`; it can block copy elision.
- Perfect forwarding (`T&&` plus `std::forward`) in templates.

## Related
- `../22_CopyElisionAndRVO`
- `../23_NoexceptAndSTL`
- `../04_ConstructorsAndDestructors`
- `../02_References`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
