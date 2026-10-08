# 26_TemplateMetaprogramming

Compile-time programming: variadic templates and fold expressions, SFINAE, CRTP, type traits, type erasure and expression SFINAE; senior template topics.

## Files
- `01_variadicTemplatesAndFoldExpressions.cpp` - sumAll with a C++17 unary fold over +
- `02_sfinaeEnableIf.cpp` - overload chosen by enable_if only for integral T
- `04_crtp.cpp` - CRTP static polymorphism without virtual dispatch
- `05_typeTraitsBasics.cpp` - is_same, enable_if, conditional
- `06_customAnyTypeErasure.cpp` - hand-rolled MyAny: type erasure behind a non-template class
- `07_sfinaeHasMember.cpp` - detect a member with void_t and declval (has_serialize<T>)
- `NOTES.md` - template metaprogramming notes and C++20 concepts

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_variadicTemplatesAndFoldExpressions.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/26_TemplateMetaprogramming/` (git-ignored).

## Key concepts / interview angles
- SFINAE: a failed substitution removes the overload instead of an error; `void_t` detects expressions.
- Fold expressions replace recursive variadic templates.
- CRTP gives static polymorphism (no vptr, inlinable) at the cost of no runtime heterogeneity.
- Type erasure = concept-based polymorphism behind a value type (`std::function`, `std::any`).
- C++20 concepts replace most SFINAE; see `../30_Cpp20Features`.

## Gotchas
- There is no `03_` file in this folder (numbering jumps from 02 to 04); that is existing, not a missing file.

## Related
- `../17_Templates`
- `../30_Cpp20Features`
- `../27_STLInternals`
- `../DesignPatterns/Behavioral/Visitor.cpp`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
