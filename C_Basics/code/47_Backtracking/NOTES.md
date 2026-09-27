# Backtracking

- The general pattern: "choose, explore, un-choose" - trial and error with
  memory of the current partial solution. At each step, make a tentative
  choice, recurse into the resulting smaller subproblem, and if that
  recursion cannot lead to a solution, undo the choice (restore state) and
  try the next candidate. This differs from brute force in that a
  partial solution already known to be invalid is abandoned immediately
  (pruned) instead of being extended all the way to a full candidate first.
- `01_nQueens.c`: choose a column for the current row, recurse to the next
  row, and prune immediately via `isSafe` (no need to ever place a
  conflicting queen and discover it later).
- `02_sudokuSolver.c`: choose a digit for the next empty cell, recurse, and
  un-choose (reset the cell to empty) if no digit leads to a full solution.
- `03_permutations.c`: choose an element to swap into the current slot,
  recurse on the remaining slots, then swap back (un-choose) before trying
  the next candidate for that slot.
- `04_subsetSum.c`: choose to include or exclude each element, recurring
  with the target reduced only along the "include" branch.
- When to reach for backtracking: combinatorial search problems over a
  solution space too large to enumerate exhaustively, but where partial
  solutions can be checked for validity early, letting whole branches of the
  search tree be pruned rather than explored to completion.
- Complexity caveat: backtracking is usually exponential in the worst case
  (e.g. N-Queens without pruning is O(n^n)), but effective pruning (as in
  `isSafe` or sudoku's row/column/box checks) makes it practical for
  reasonably-sized real inputs, even though the theoretical worst case
  remains exponential.
