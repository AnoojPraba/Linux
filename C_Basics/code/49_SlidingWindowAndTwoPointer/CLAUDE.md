# 49_SlidingWindowAndTwoPointer

Two-pointer and sliding-window techniques for linear-time array/string problems.

## Files
- `01_twoPointerPairSum.c` - opposite-end pointers finding a pair with a target sum in a sorted array
- `02_fixedWindowMaxSum.c` - fixed-size window, slide by adding the new and dropping the old element
- `03_longestUniqueSubstring.c` - variable window expanded and shrunk with a last-seen table
- `NOTES.md` - preconditions and the recognition pattern for interviews

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_twoPointerPairSum.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/49_SlidingWindowAndTwoPointer/` (git-ignored).

## Key concepts / interview angles
- Opposite-direction two pointers need sorted input (or a monotonic property).
- Fixed window: O(n) via running sum instead of recomputing O(nk).
- Variable window: expand right, shrink left while the invariant is broken.
- Each pointer moves at most n times, so total O(n).

## Related
- `../50_ClassicArrayAndStringProblems`
- `../10_Search_alg`
- `../11_StringPatternMatching`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
