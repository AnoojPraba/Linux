# 47_Backtracking

Backtracking (choose, explore, un-choose): N-Queens, Sudoku, permutations and subset sum.

## Files
- `01_nQueens.c` - row-by-row queen placement with column/diagonal checks, prints a solution
- `02_sudokuSolver.c` - fill empty cells with isValidPlacement checks
- `03_permutations.c` - generate permutations by swapping elements into position
- `04_subsetSum.c` - include/exclude recursion with a chosen-marker array
- `NOTES.md` - the choose/explore/un-choose pattern, when to use it, exponential complexity caveat

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_nQueens.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/47_Backtracking/` (git-ignored).

## Key concepts / interview angles
- Prune early: reject a partial solution as soon as it violates a constraint.
- State must be restored after the recursive call (un-choose).
- Worst case is exponential; add constraint propagation or ordering heuristics (e.g. most constrained cell first for Sudoku).
- Know when DP (overlapping subproblems) beats backtracking.

## Related
- `../46_DynamicProgramming`
- `../14_Recursion`
- `../48_GreedyAlgorithms`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
