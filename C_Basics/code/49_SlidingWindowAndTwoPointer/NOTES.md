# Sliding Window and Two Pointer

- Two pointer (`01_twoPointerPairSum.c`): typically needs a precondition -
  usually sorted input, or some other structural property - that lets you
  safely discard a whole side of the search space each step (moving left
  forward can only increase the sum, moving right backward can only
  decrease it). Without that property (unsorted input), you'd need a hash
  set instead to stay at O(n) time, trading O(1) space for O(n) space.
- Sliding window (`02_fixedWindowMaxSum.c`, `03_longestUniqueSubstring.c`):
  the technique is fundamentally about avoiding recomputation for
  contiguous-subarray/substring problems. A fixed-size window turns an
  O(n*k) brute force (recomputing each window's sum) into O(n) by
  incrementally updating the running value as the window slides. A
  variable-size window turns an O(n^2) brute force (checking every
  substring) into O(n) by only ever expanding right and shrinking left,
  each pointer moving forward at most n times total.
- Recognition pattern for interviews: reach for two pointer / sliding window
  when the problem mentions "contiguous subarray/substring", "pair/triplet
  in a sorted array", "at most K distinct characters", "longest/shortest
  window satisfying some condition", or similar - these are strong signals
  that an O(n) or O(n log n) pointer-based approach exists instead of the
  naive O(n^2) or worse.
