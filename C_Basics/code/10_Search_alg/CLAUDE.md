# 10_Search_alg

Searching: linear, iterative binary, ternary search and the second-largest-element problem, with a note on when ternary search is actually useful.

## Files
- `01_Second_Largest.c` - single-pass second largest element tracking max and sec_max
- `02_LinearSearch.c` - O(n) scan, no ordering assumption
- `03_BinarySearchIterative.c` - O(log n) on a sorted array, halving the range each iteration
- `04_ternarySearch.c` - splits the range in three (two comparisons per level)
- `05_selfOrganizingAndSentinelSearch.c` - Linear-search improvements: sentinel search, move-to-front and transpose heuristics for skewed access patterns.
- `06_binarySearchVariants.c` - The binary search family on sorted arrays: lower/upper bound, first and last occurrence, count of a value, insert position and floor/ceiling.
- `07_fibonacciSearch.c` - Fibonacci search: divide a sorted array with Fibonacci numbers using only addition and subtraction (useful when division is costly).
- `08_rotatedArraySearch.c` - Searching a sorted array that was rotated: find the rotation point (minimum) and search for a value in O(log n), including duplicates.
- `09_peakElementAndMountain.c` - Binary search without a target: find a peak element, the peak of a mountain array and the single element in a sorted array of pairs.
- `10_search2dMatrix.c` - Searching a 2D matrix: staircase search from the top-right corner (rows and columns sorted) and binary search over a fully sorted matrix.
- `11_bisectionAndAnswerSearch.c` - Binary search on the answer: integer square root, bisection root finding, minimum eating speed and the minimum ship capacity.
- `NOTES.md` - why ternary search is not better than binary on sorted arrays; its real use is the extremum of a unimodal function

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_Second_Largest.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/10_Search_alg/` (git-ignored).

## Key concepts / interview angles
- Binary search needs sorted input; compute `mid = low + (high - low) / 2` to avoid overflow.
- Ternary search does 2 comparisons per level vs 1 for binary, so no practical gain on sorted arrays; use it for unimodal max/min.
- Second largest in one pass: update `max` and `sec_max` together, watch duplicates.

## Related
- `../14_Recursion/03_recursiveBinarySearch.c` - recursive form
- `../21_QsortBsearch` - stdlib bsearch
- `../12_Sorting` - sorting prerequisite
- `../11_StringPatternMatching`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
