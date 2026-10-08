# 14_Recursion

Recursion fundamentals: factorial, Fibonacci (naive exponential) and recursive binary search, contrasted with iterative forms.

## Files
- `01_factorial.c` - recursive and iterative factorial (n=6)
- `02_fibonacci.c` - fibNaive (O(2^n)) vs fibMemo (memoised with a cache array)
- `03_recursiveBinarySearch.c` - binary search with low/high indices

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_factorial.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/14_Recursion/` (git-ignored).

## Key concepts / interview angles
- Every recursion needs a base case and progress toward it; each call costs a stack frame (depth limit).
- Naive Fibonacci recomputes subproblems: O(2^n); memoisation/DP makes it O(n) (see `../46_DynamicProgramming`).
- Tail recursion is not guaranteed to be optimised in C; at -O2 GCC often does it, do not rely on it.
- Factorial overflows `long` quickly; mention it.

## Related
- `../46_DynamicProgramming`
- `../47_Backtracking`
- `../10_Search_alg`
- `../79_StackFramesAndCallingConvention` - what a frame costs

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
