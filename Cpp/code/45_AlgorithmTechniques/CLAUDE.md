# 45_AlgorithmTechniques

Algorithm design techniques in modern C++: prefix sums, sliding windows, DP, backtracking, branch and bound, divide and conquer, randomized algorithms.

## Files
- `01_prefixSumsAndWindows.cpp` - 1-D/2-D prefix sums, Kadane, sliding-window maximum with a deque, longest substring without repeats.
- `02_dynamicProgramming.cpp` - 0/1 knapsack (1-D table), LCS with reconstruction, edit distance (two rows), memoized climbing.
- `03_backtracking.cpp` - N-Queens with bitmasks, permutations with next_permutation, subset sum with pruning.
- `04_branchAndBound.cpp` - Best-first 0/1 knapsack with a fractional bound compared with brute force.
- `05_divideAndConquer.cpp` - Fast power mod, inversion counting, maximum subarray by merging four-number summaries.
- `06_randomizedAlgorithms.cpp` - Reservoir sampling, randomized quickselect, Miller-Rabin (deterministic bases for 64-bit).
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `g++ -std=c++20 -Wall -Wextra -pthread 01_prefixSumsAndWindows.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/45_AlgorithmTechniques/` (git-ignored).

## Key concepts / interview angles
- **Prefix sums:** `sum(l..r) = pre[r+1] - pre[l]`; 2-D by inclusion-exclusion; hash map of prefix sums counts subarrays with sum k in O(n). **Kadane:** best sum ending here = max(a[i], cur + a[i]); start `best` at `a[0]`. **Sliding window max:** deque of indexes with decreasing values, O(n). **Variable window:** longest substring without repeats - left jumps past the previous occurrence.
- **DP:** state, recurrence, base case, order. Top-down memoization vs bottom-up tabulation. Knapsack loops capacity **downwards** (each item once; upwards = unbounded). LCS O(nm), reconstruct by walking back; edit distance keeps two rows. DP when sub-problems overlap; greedy only when the local choice is provably safe (coin change 4/3/1 breaks greedy).
- **Backtracking:** choose, explore, un-choose, prune early. N-Queens bitmasks: `free & -free` picks the lowest free column; shifts move diagonals. `next_permutation` walks permutations lexicographically from a sorted start.
- **Branch and bound:** optimistic bound + prune against the incumbent; best-first via priority_queue; tight bound visits a tiny part of the 2^n tree.
- **Divide and conquer:** master theorem; inversions in merge sort; max subarray via (sum, best, prefix, suffix); fast power is O(log n).

## Gotchas
- `engine() % n` is slightly biased; used only so outputs are identical across standard libraries.
- Overflow in knapsack/DP tables: use wide types.
