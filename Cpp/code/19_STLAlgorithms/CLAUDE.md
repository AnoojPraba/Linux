# 19_STLAlgorithms

Core <algorithm>/<numeric> usage on a vector: sort, find, transform, accumulate.

## Files
- `01_algorithms.cpp` - sort, find, transform and accumulate on a vector<int>

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_algorithms.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/19_STLAlgorithms/` (git-ignored).

## Key concepts / interview angles
- Algorithms operate on iterator ranges, decoupled from containers; pair with lambdas.
- Complexity guarantees: `sort` O(n log n), `find` O(n), `lower_bound` O(log n) on sorted ranges.
- Erase-remove idiom; `accumulate` init-value type determines the result type (pitfall with `0` vs `0.0`).
- C++20 ranges give composable pipelines.

## Related
- `../16_Lambdas`
- `../18_STLContainers`
- `../30_Cpp20Features`
- `../../../C_Basics/code/12_Sorting`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
