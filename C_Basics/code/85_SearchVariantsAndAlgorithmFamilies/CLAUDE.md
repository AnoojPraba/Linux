# 85_SearchVariantsAndAlgorithmFamilies

Search variants beyond binary search, divide and conquer, branch and bound and randomized algorithms.

## Files
- `01_jumpExponentialInterpolation.c` - Jump (O(sqrt n)), exponential/galloping (O(log i)) and interpolation (O(log log n) for uniform data) search compared by probe count.
- `02_divideAndConquer.c` - Fast exponentiation mod m, inversion counting inside merge sort, and the master-theorem examples.
- `03_knapsackBranchAndBound.c` - 0/1 knapsack with a fractional upper bound; prints how many nodes of the full tree were visited.
- `04_randomizedAlgorithms.c` - Reservoir sampling and randomized quickselect with a deterministic generator.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_jumpExponentialInterpolation.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/85_SearchVariantsAndAlgorithmFamilies/` (git-ignored).

## Key concepts / interview angles
- All of these searches need **sorted, random-access** data. **Binary** O(log n); **jump** O(sqrt n) (block jumps then a linear scan, only moves forward); **exponential/galloping** O(log i) for key position i (probe 1,2,4,8,.. then binary search; the way to search an unbounded or streamed sorted sequence); **interpolation** guesses `lo + (key-a[lo])*(hi-lo)/(a[hi]-a[lo])`, ~O(log log n) for uniform values but O(n) for skewed data. Guard a zero denominator and the product overflow.
- Midpoint as `lo + (hi - lo) / 2`, not `(lo + hi) / 2`.
- **Divide and conquer:** divide, conquer recursively, combine; `T(n) = a T(n/b) + f(n)` and the master theorem (merge sort Theta(n log n), binary search Theta(log n), Strassen Theta(n^2.81)). Sub-problems do not overlap (unlike DP). Inversions = merge sort + one counting line, O(n log n) instead of O(n^2). Fast power halves the exponent: O(log n) multiplications.
- **Branch and bound:** search the decision tree but prune a branch when its optimistic **bound** cannot beat the best solution so far (for knapsack the bound is the fractional-knapsack value). Quality of the bound decides everything; items sorted by value/weight make it tight. Best-first order (priority queue by bound) prunes more than DFS. Relatives: backtracking prunes on infeasibility only; DP is O(n*cap).
- **Randomized:** Las Vegas (always correct, random running time: quickselect, random-pivot quicksort) vs Monte Carlo (bounded time, small error probability: Miller-Rabin, Bloom filter). **Reservoir sampling** picks k of an unknown-length stream in one pass with O(k) memory: item i replaces a random slot with probability k/(i+1). **Quickselect** is expected O(n).

## Gotchas
- Interpolation search degrades to O(n) on skewed data (the demo shows it).
- The demo generator is a tiny LCG so output is repeatable; use a real RNG in production.
