# 46_DynamicProgramming

Dynamic programming staples: memoisation vs tabulation, 0/1 knapsack, LCS, LIS, coin change, edit distance and Kadane.

## Files
- `01_fibMemoVsTabulation.c` - top-down with cache vs bottom-up table, contrasted with naive O(2^n)
- `02_knapsack.c` - 0/1 knapsack table[i][w]; each row only needs the previous one
- `03_longestCommonSubsequence.c` - LCS table over two strings
- `04_longestIncreasingSubsequence.c` - O(n^2) DP (and a faster variant)
- `05_coinChangeMinCoins.c` - minimum coins, unbounded knapsack variant
- `06_editDistance.c` - Levenshtein 2D DP
- `07_kadaneMaxSubarray.c` - O(n) maximum subarray sum
- `NOTES.md` - notes on LIS, coin change, edit distance, Kadane

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_fibMemoVsTabulation.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/46_DynamicProgramming/` (git-ignored).

## Key concepts / interview angles
- Recipe: define state, recurrence, base cases, iteration order; then optimise space (rolling rows).
- Memoisation (top-down, lazy, recursion depth) vs tabulation (bottom-up, iterative).
- 0/1 knapsack is pseudo-polynomial O(nW).
- Coin change DP is correct where greedy is not (contrast `../48_GreedyAlgorithms/03_coinChangeGreedy.c`).
- LIS has an O(n log n) patience-sorting variant.

## Related
- `../48_GreedyAlgorithms`
- `../14_Recursion/02_fibonacci.c`
- `../47_Backtracking`
- `../50_ClassicArrayAndStringProblems/10_climbingStairs.c`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
