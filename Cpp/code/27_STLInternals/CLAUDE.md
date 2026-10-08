# 27_STLInternals

How STL containers behave internally: vector growth, iterator invalidation, map vs unordered_map and writing a custom iterator.

## Files
- `01_vectorGrowthStrategy.cpp` - prints size vs capacity after each push_back
- `02_iteratorInvalidation.cpp` - reallocation invalidates iterators/pointers/references; other invalidation cases
- `03_mapVsUnorderedMap.cpp` - red-black tree map vs hash map iteration order and rough timing
- `04_customIterator.cpp` - fixed-capacity container with an input-style iterator usable by range-for and algorithms
- `NOTES.md` - growth factor, invalidation rules, map vs unordered_map, custom iterator minimum

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_vectorGrowthStrategy.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/27_STLInternals/` (git-ignored).

## Key concepts / interview angles
- Amortised O(1) `push_back` via geometric growth (2x in libstdc++, 1.5x in MSVC).
- Iterator invalidation rules differ per container: vector, deque, list, map, unordered_map.
- `std::map` is a red-black tree (ordered, stable iterators); `unordered_map` is a bucket hash table.
- Minimal iterator: `operator*`, `++`, `!=` (and the iterator traits for algorithms).

## Gotchas
- Timing output in 03 is machine-specific and rough.

## Related
- `../18_STLContainers`
- `../19_STLAlgorithms`
- `../../../C_Basics/code/34_HashTable`
- `../../../C_Basics/code/39_SelfBalancingTrees`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
