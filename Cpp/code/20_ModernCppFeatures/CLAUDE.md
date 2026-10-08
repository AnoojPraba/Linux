# 20_ModernCppFeatures

C++11-C++20 language and library features every modern codebase uses: auto/decltype, structured bindings, constexpr, optional/variant, string_view, rule of five, std::visit.

## Files
- `01_autoDecltypeTrailingReturn.cpp` - addValues with trailing decltype return type
- `02_rangeForAndStructuredBindings.cpp` - range-for over vector/map; structured bindings for pairs
- `03_constexprAndIfConstexpr.cpp` - compile-time factorial; `if constexpr`
- `04_optionalVariantAny.cpp` - safeDivide returning optional; variant and any
- `05_stringView.cpp` - startsWith with string_view (no copies)
- `06_ruleOfFive.cpp` - resource owner with all five special members and move
- `07_stdVisitVariant.cpp` - overloaded-lambda helper with std::visit
- `NOTES.md` - C++11-C++20 summary; C++20 ranges and concepts

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_autoDecltypeTrailingReturn.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/20_ModernCppFeatures/` (git-ignored).

## Key concepts / interview angles
- `optional` replaces sentinel values or exceptions for absence; `variant` + `visit` is a type-safe tagged union.
- `string_view` is non-owning: never return one into a temporary.
- `constexpr` computes at compile time when arguments are constants; `if constexpr` discards branches in templates.
- Rule of five: custom dtor implies defining or deleting copy and move operations.
- Structured bindings unpack pairs, tuples and structs.

## Related
- `../21_MoveSemantics`
- `../30_Cpp20Features`
- `../34_SpanStringViewAndPmr`
- `../04_ConstructorsAndDestructors`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
