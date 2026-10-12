# AlgorithmTechniques

- **Prefix sums:** `sum(l..r) = pre[r+1] - pre[l]`; 2-D by inclusion-exclusion; hash map of prefix sums counts subarrays with sum k in O(n). **Kadane:** best sum ending here = max(a[i], cur + a[i]); start `best` at `a[0]`. **Sliding window max:** deque of indexes with decreasing values, O(n). **Variable window:** longest substring without repeats - left jumps past the previous occurrence.
- **DP:** state, recurrence, base case, order. Top-down memoization vs bottom-up tabulation. Knapsack loops capacity **downwards** (each item once; upwards = unbounded). LCS O(nm), reconstruct by walking back; edit distance keeps two rows. DP when sub-problems overlap; greedy only when the local choice is provably safe (coin change 4/3/1 breaks greedy).
- **Backtracking:** choose, explore, un-choose, prune early. N-Queens bitmasks: `free & -free` picks the lowest free column; shifts move diagonals. `next_permutation` walks permutations lexicographically from a sorted start.
- **Branch and bound:** optimistic bound + prune against the incumbent; best-first via priority_queue; tight bound visits a tiny part of the 2^n tree.
- **Divide and conquer:** master theorem; inversions in merge sort; max subarray via (sum, best, prefix, suffix); fast power is O(log n).
- **Randomized:** Las Vegas vs Monte Carlo; reservoir sampling; quickselect; Miller-Rabin with bases {2..37} is deterministic below 3.3e24 (`unsigned __int128` for mulmod).
