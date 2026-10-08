# 07_OperatorOverloading

Operator overloading on a 2D vector: +, ==, <<, and [].

## Files
- `01_vector2d.cpp` - Vector2D with member operator+, operator==, operator[] and stream operator<< (friend/free function)

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_vector2d.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/07_OperatorOverloading/` (git-ignored).

## Key concepts / interview angles
- Overload with natural semantics only; keep `==` consistent with other comparisons.
- `operator<<` must be a free (often friend) function because the left operand is `ostream`.
- Binary arithmetic as free functions keeps implicit conversions symmetric; return by value.
- `operator[]` needs const and non-const overloads.
- C++20 `<=>` can generate comparisons.

## Related
- `../06_FriendFunctionsAndClasses`
- `../04_ConstructorsAndDestructors`
- `../30_Cpp20Features`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
