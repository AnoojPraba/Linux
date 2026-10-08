# 50_ClassicArrayAndStringProblems

Thirteen frequently asked array and string interview problems with idiomatic C solutions.

## Files
- `01_twoSumHashmap.c` - Two Sum with an inlined hash table (see 34_HashTable for the reusable one)
- `02_palindromeChecks.c` - valid palindrome ignoring case/non-alphanumerics (two pointers) and number variants
- `03_anagramCheck.c` - 26-counter frequency approach; sorting alternative noted
- `04_groupAnagrams.c` - sorted-character signature as the grouping key
- `05_mergeIntervals.c` - sort by start then sweep and extend
- `06_rotateArray.c` - rotate right by k with three reversals, O(1) space
- `07_moveZeroes.c` - single-pass write-pointer, stable
- `08_spiralMatrix.c` - four shrinking boundaries
- `09_bestTimeToBuySellStock.c` - track min price so far, O(n)
- `10_climbingStairs.c` - Fibonacci recurrence
- `11_trappingRainWater.c` - two pointers with running left/right max
- `12_containerWithMostWater.c` - two pointers from both ends moving the shorter side
- `13_firstNonRepeatingChar.c` - two-pass frequency count
- `NOTES.md` - one paragraph per problem: approach and complexity

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_twoSumHashmap.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/50_ClassicArrayAndStringProblems/` (git-ignored).

## Key concepts / interview angles
- Trade a hash table or counting array for O(n) time instead of O(n^2).
- Three reversals rotate an array in place; reverse(0..n-1), then each part.
- Trapping rain water and container-with-most-water both use two pointers moving the limiting side.
- Sort-then-sweep pattern for intervals; mention the sort dominates.
- Always state time/space complexity and edge cases (empty, size 1, duplicates, overflow).

## Related
- `../49_SlidingWindowAndTwoPointer`
- `../34_HashTable`
- `../46_DynamicProgramming`
- `../12_Sorting`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
