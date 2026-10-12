# 38_SearchingAlgorithms

Searching sorted data in modern C++: binary search family and the jump, exponential and interpolation variants.

## Files
- `01_searchVariants.cpp` - binary, jump, exponential and interpolation search over std::span.
- `02_linearSentinelAndSelfOrganizing.cpp` - Linear search and its improvements: std::find family, sentinel search, move-to-front and transpose heuristics.
- `03_binarySearchFamily.cpp` - The binary search family by hand and in the STL: lower/upper bound, equal_range, first/last occurrence, counts and insert position.
- `04_stlSearchAlgorithms.cpp` - The STL searching toolbox: find, adjacent_find, search, find_end, find_first_of, mismatch, includes and set/map lookups.
- `05_ternaryAndFibonacciSearch.cpp` - Ternary search for the extremum of a unimodal function (real and integer) and Fibonacci search on a sorted array.
- `06_rotatedArrayAndPeak.cpp` - Binary search without an exact target: rotated sorted arrays (min and search), peak element and the single element in sorted pairs.
- `07_search2dMatrix.cpp` - Searching matrices: staircase search for row/column-sorted matrices and one binary search for a fully sorted matrix.
- `08_binarySearchOnAnswer.cpp` - Binary search on the answer with a monotone predicate: integer sqrt, minimum eating speed, ship capacity and a generic helper.
- `09_searchInUnboundedSequence.cpp` - Searching a sorted sequence of unknown length: exponential (galloping) search over a generator function.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `g++ -std=c++20 -Wall -Wextra -pthread 01_searchVariants.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/38_SearchingAlgorithms/` (git-ignored).

## Key concepts / interview angles
- **`lower_bound`** = first element >= value, **`upper_bound`** = first element > value, difference = count of equals; need sorted data; O(log n) on random-access iterators, O(n) iterator steps on a `list`. `nth_element` = quickselect, O(n) average. `binary_search` only answers present/absent.
- **Search variants** (sorted, indexable): jump O(sqrt n), exponential O(log i) (unbounded/streams), interpolation O(log log n) uniform but O(n) skewed. Midpoint `lo + (hi - lo) / 2`.

## Gotchas
- Interpolation search is O(n) on skewed data (the demo shows it).
