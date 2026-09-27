# Dynamic Programming

- Longest Increasing Subsequence (`04_longestIncreasingSubsequence.c`): the
  classic O(n^2) DP (`lengthAt[i]` = best LIS ending at i, scanning all
  earlier j) is simple and easy to reason about. The O(n log n)
  patience-sorting/binary-search approach (`tails[]`) trades that simplicity
  for speed - it only reconstructs the *length*, not the subsequence itself,
  without extra bookkeeping. Know both: the O(n^2) version as the intuitive
  baseline, the O(n log n) version as the "can you do better?" follow-up.
- Coin Change - min coins (`05_coinChangeMinCoins.c`): framed as an
  *unbounded* knapsack, contrasted with `02_knapsack.c`'s *0/1* knapsack.
  0/1 knapsack's recurrence only ever looks at row i-1 (each item usable at
  most once); unbounded coin change reuses the same row/array position
  (`best[a - coins[i]]`), since a coin denomination can be reused any number
  of times. Same DP skeleton, different reuse rule.
- Edit Distance (`06_editDistance.c`): the textbook 2D Levenshtein DP.
  Beyond being an interview staple, this exact recurrence underlies real
  tools - diff utilities (minimal edit script between two file versions),
  spell-checkers (closest dictionary word by edit distance), and
  bioinformatics (minimal mutations to align two DNA/protein sequences).
- Kadane's Algorithm (`07_kadaneMaxSubarray.c`): maximum subarray sum in a
  single O(n) pass. It's technically a tiny two-state DP (best-ending-here
  vs. best-overall), but it gets taught and asked about as its own named
  algorithm because it is such a common, standalone interview question -
  worth recognizing on sight rather than re-deriving from scratch.
