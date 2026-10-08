# 16_Lambdas

Lambda syntax, capture by value/reference, and use with STL algorithms.

## Files
- `01_lambdas.cpp` - named lambda predicate, capture-by-reference accumulator with for_each, capture-by-value scaler, comparator for sort

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_lambdas.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/16_Lambdas/` (git-ignored).

## Key concepts / interview angles
- A lambda is an unnamed class with `operator()`; captures become data members.
- `[=]`/`[&]` default captures hide lifetime bugs; capture dangling references only if the lambda outlives the scope.
- Lambdas are `const` by default; `mutable` allows modifying by-value captures.
- Generic lambdas (`auto` params) and init-captures (`[p = std::move(x)]`) in C++14; templated lambdas in C++20.
- Storing in `std::function` costs type erasure (heap/indirection).

## Related
- `../19_STLAlgorithms`
- `../20_ModernCppFeatures`
- `../DesignPatterns/Behavioral/Strategy.cpp`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
