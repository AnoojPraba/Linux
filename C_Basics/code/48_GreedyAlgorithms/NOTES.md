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

## More algorithms in this folder
- **Greedy:** make the locally best choice and never reconsider. Correct only with **optimal substructure + greedy-choice property**; prove it by an *exchange argument* (swap an optimal solution's first choice for the greedy one without making it worse) or by staying-ahead. Counterexamples: coin change 4/3/1, 0/1 knapsack by ratio - use DP there.
- **Job sequencing with deadlines:** sort by profit descending, put each job in the **latest free slot** at or before its deadline; union-find over slots makes the slot lookup near O(1). O(n log n).
- **Huffman coding:** repeatedly merge the two least-frequent subtrees (min-heap); the tree gives an optimal **prefix-free** code, expected length within 1 bit of entropy. Same merge rule solves **optimal merge pattern** and **connect ropes** in O(n log n).
- **Intervals:** activity selection (earliest finish), **minimum platforms / meeting rooms** (sort starts and ends separately and sweep, or a min-heap of end times), **minimum arrows/stabbing points** (sort by end, shoot at the end of the first uncovered interval), **cover a segment** (among intervals starting inside the covered part, take the farthest reach).
- **Scans:** gas station (if total gas >= total cost, the start is right after the last point where the running tank went negative), jump game (track the farthest reachable index; min jumps = BFS levels over a window), best time to buy/sell stock, trapping water.
- **Two-pass greedy:** candy distribution (left-to-right then right-to-left), assign cookies (sort both, smallest sufficient cookie to least greedy child).
- **Digit strings:** remove k digits for the smallest number uses a **monotonic stack** (pop a larger digit when a smaller one arrives); largest number from a list sorts with the comparator `a+b > b+a` (concatenation order).
- **Scheduling:** with cooldown n the time is `max(len, (maxfreq-1)*(n+1) + number_of_tasks_with_maxfreq)`; shortest-job-first minimizes average waiting time; earliest deadline first is optimal for feasibility on one processor.
