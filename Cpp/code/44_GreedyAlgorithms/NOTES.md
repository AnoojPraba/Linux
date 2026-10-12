# GreedyAlgorithms

- **Greedy:** earliest-finish-first is optimal for interval scheduling (cut property style argument).

## More algorithms in this folder
- **Greedy:** locally best choice, never reconsidered. Needs **optimal substructure + greedy-choice property**; prove by an exchange argument. Works: activity selection, fractional knapsack, Huffman, MST (Kruskal/Prim), Dijkstra (non-negative), interval scheduling, job sequencing. Fails: 0/1 knapsack by ratio, coin change with arbitrary denominations (4/3/1) - use DP.
- **Fractional knapsack:** sort by value/weight, take whole items then a fraction of the last: optimal. **0/1 knapsack** needs DP. **Coins:** greedy is optimal only for canonical systems (US coins), counterexample 4/3/1.
- **Job sequencing:** sort by profit, place each job in the latest free slot <= deadline; union-find over slots makes it near-linear.
- **Huffman:** min-heap of subtrees, merge the two rarest repeatedly; the tree yields an optimal prefix-free code; decode by walking the tree. Same rule: optimal merge pattern and connect-ropes.
- **Intervals:** merge (sort by start, extend the last), **minimum arrows/stabbing points** and **maximum non-overlapping** (sort by end), **minimum rooms/platforms** (sweep over sorted starts/ends or a heap of end times), cover a segment (farthest reach among intervals starting inside).
- **Scans:** gas station (restart after the running tank goes negative; total >= 0 guarantees the candidate works), jump game (farthest reachable; min jumps by windows), stock trades (single: running minimum; unlimited: sum of positive differences).
- **Distribution:** candy (two passes), cookies and boats (sort + two pointers), digit strings (monotonic stack to remove k digits; sort by concatenation `a+b > b+a` for the largest number).
- **Scheduling:** SJF minimizes average waiting; earliest deadline first is optimal for single-machine feasibility; LPT is a 4/3-approximation for makespan on parallel machines; cooldown formula `max(n_tasks, (max_freq-1)(n+1) + count_of_max)`.
