# Classic Array and String Problems

A cheat-sheet of extremely commonly-asked array/string interview problems -
terse reminders of the technique and complexity, not full explanations.

- **Two Sum (hashmap)** (`01_twoSumHashmap.c`): for each element, check if
  `target - element` was already seen in a hash table. O(n) time, O(n)
  space, works on UNSORTED input. Contrast with the sorted two-pointer
  version in `49_SlidingWindowAndTwoPointer/01_twoPointerPairSum.c`, which
  needs sorted input but avoids extra space. See `34_HashTable` for a
  from-scratch hash table.
- **Palindrome checks** (`02_palindromeChecks.c`): string version uses
  two pointers from both ends, skipping non-alphanumeric chars and
  ignoring case. Integer version reverses digits arithmetically (no string
  conversion) via repeated `%10`/`/10`.
- **Anagram check** (`03_anagramCheck.c`): 26-count character-frequency
  comparison, O(n). Contrast with sorting both strings and comparing,
  O(n log n).
- **Group Anagrams** (`04_groupAnagrams.c`): sorted-string signature as a
  grouping key; strings sharing a signature are anagrams of each other.
- **Merge Overlapping Intervals** (`05_mergeIntervals.c`): sort by start,
  sweep and extend the current merged interval's end whenever the next
  interval's start is <= that end.
- **Rotate Array** (`06_rotateArray.c`): "reverse three times" trick -
  reverse whole array, then reverse first k, then reverse the rest. O(1)
  extra space, in-place.
- **Move Zeroes** (`07_moveZeroes.c`): single-pass two-pointer, a "write"
  index tracking where the next non-zero belongs; preserves relative
  order of non-zero elements.
- **Spiral Matrix Traversal** (`08_spiralMatrix.c`): maintain
  top/bottom/left/right boundaries, traverse each side, shrink the
  boundary just walked.
- **Best Time to Buy and Sell Stock** (`09_bestTimeToBuySellStock.c`):
  single pass tracking min-price-so-far and best profit if selling today.
- **Climbing Stairs** (`10_climbingStairs.c`): DP/Fibonacci in disguise -
  `ways(n) = ways(n-1) + ways(n-2)`. See `46_DynamicProgramming` for the
  general memoization/tabulation pattern.
- **Trapping Rain Water** (`11_trappingRainWater.c`): two pointers
  tracking max-from-left/max-from-right; move the side with the smaller
  max, since water there is bounded by that smaller max.
- **Container With Most Water** (`12_containerWithMostWater.c`): two
  pointers from both ends, always move the SHORTER line inward - moving
  the taller one can never improve the area since it's already not the
  limiting factor.
- **First Non-Repeating Character** (`13_firstNonRepeatingChar.c`):
  frequency count pass, then a second pass to find the first count-of-1
  character.
