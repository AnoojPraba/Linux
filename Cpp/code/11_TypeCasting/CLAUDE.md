# 11_TypeCasting

The four named C++ casts and when each is appropriate.

## Files
- `01_casts.cpp` - static_cast (numeric/related types), dynamic_cast (checked downcast), const_cast, reinterpret_cast

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_casts.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/11_TypeCasting/` (git-ignored).

## Key concepts / interview angles
- `static_cast`: compile-time checked, for well-defined conversions and upcasts/downcasts you know are valid.
- `dynamic_cast`: runtime checked via RTTI; returns nullptr (pointer) or throws `bad_cast` (reference); needs a polymorphic type.
- `const_cast`: remove const only to call legacy APIs; modifying an originally-const object is UB.
- `reinterpret_cast`: bit reinterpretation; easy to violate strict aliasing, use `memcpy`/`std::bit_cast`.
- Prefer avoiding casts; C-style casts hide which one you are doing.

## Related
- `../10_Polymorphism`
- `../../../C_Basics/code/62_StrictAliasing`
- `../30_Cpp20Features`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
