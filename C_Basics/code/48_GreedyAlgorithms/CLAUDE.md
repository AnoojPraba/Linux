# 48_GreedyAlgorithms

Greedy algorithms: activity selection, fractional knapsack, and a coin-change greedy that demonstrates where greedy fails against DP.

## Files
- `01_activitySelection.c` - sort by end time, pick earliest-finishing compatible activity
- `02_fractionalKnapsack.c` - sort by value/weight ratio, take fractions
- `03_coinChangeGreedy.c` - largest-coin-first; optimal for canonical coin systems only
- `04_candyAndCookies.c` - Two-pass greedy distribution problems: minimum candies for rating neighbours and assigning cookies to children.
- `05_gasStationAndJumpGame.c` - Two classic greedy scans: gas station circuit (where to start) and jump game (can you reach the end, minimum jumps).
- `06_jobSequencingWithDeadlines.c` - Job sequencing with deadlines: maximize profit by scheduling the most profitable jobs in the latest free slot, plus the union-find speedup.
- `07_meetingRoomsAndPlatforms.c` - Minimum meeting rooms / railway platforms: sort start and end times and sweep, or use a min-heap of end times.
- `08_minArrowsAndIntervalCover.c` - Interval greedy problems: minimum arrows to burst balloons, minimum points that stab all intervals, and covering a segment with the fewest intervals.
- `09_removeKDigitsAndLargestNumber.c` - Greedy on digit strings: remove k digits for the smallest number (monotonic stack) and arrange numbers to form the largest value.
- `10_optimalMergePattern.c` - Optimal merge pattern: merge sorted files in the cheapest order by repeatedly merging the two smallest (same greedy as Huffman), also connecting ropes.
- `11_huffmanCoding.c` - Huffman coding: build the optimal prefix code with a min-heap of subtrees, print the codes and the compression ratio.
- `12_taskSchedulerCooldown.c` - Task scheduler with a cooldown: the minimum time to run tasks when equal tasks must be n slots apart (counting formula and heap simulation).
- `13_greedyCounterexamples.c` - When greedy fails: coin change with 1/3/4, 0/1 knapsack by ratio and the DP answers that fix them.
- `NOTES.md` - what makes a problem greedy-solvable (optimal substructure plus greedy choice property)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_activitySelection.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/48_GreedyAlgorithms/` (git-ignored).

## Key concepts / interview angles
- Greedy needs the greedy-choice property; justify with an exchange argument.
- Fractional knapsack is greedy; 0/1 knapsack is not (needs DP).
- Greedy coin change fails for e.g. {1,3,4} making 6; see DP version in `../46_DynamicProgramming/05_coinChangeMinCoins.c`.
- Typical complexity is dominated by the sort, O(n log n).

## Related
- `../46_DynamicProgramming`
- `../43_Graph/05_kruskalMST.c` - greedy MST
- `../12_Sorting`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
