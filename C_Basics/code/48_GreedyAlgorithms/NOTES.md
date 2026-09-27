# Greedy Algorithms

- What makes a problem greedy-solvable: optimal substructure (an optimal
  solution to the whole problem contains optimal solutions to its
  subproblems) plus the "greedy choice property" - a locally optimal choice
  made now, without reconsidering it later, still leads to a globally
  optimal solution. Optimal substructure alone isn't enough (DP problems
  have it too); the greedy choice property must be proven for each specific
  problem, not assumed just because a greedy strategy is easy to state.
- `01_activitySelection.c`: sort by end time, always pick the earliest-
  ending compatible activity. Exchange-argument sketch - any optimal
  schedule can be transformed to start with the earliest-ending activity
  without reducing the count, since an earlier end time only leaves more
  room for subsequent picks.
- `02_fractionalKnapsack.c`: sort by value/weight ratio, take as much of the
  best ratio as fits. Contrast with 0/1 knapsack
  (`46_DynamicProgramming/02_knapsack.c`): fractional takeability is exactly
  what makes greedy-by-ratio optimal here, and exactly what's missing in 0/1
  knapsack, where greedy can lose to a DP solution (see the file's header
  comment for the classic counterexample).
- `03_coinChangeGreedy.c`: greedy (largest coin that fits) is optimal for
  "canonical" coin systems like US coins, but fails for some non-canonical
  systems - the classic counterexample being coins {1, 3, 4} with target 6,
  where greedy gives 3 coins (4+1+1) but the optimal answer is 2 (3+3). This
  is THE recurring interview trap: don't assume greedy works without
  checking the greedy choice property for the specific coin system (or
  problem) in front of you - fall back to the general-purpose DP solution
  (`46_DynamicProgramming/05_coinChangeMinCoins.c`) whenever it isn't known
  to be canonical.
