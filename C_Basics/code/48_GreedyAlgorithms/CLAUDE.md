# 48_GreedyAlgorithms

Greedy algorithms: activity selection, fractional knapsack, and a coin-change greedy that demonstrates where greedy fails against DP.

## Files
- `01_activitySelection.c` - sort by end time, pick earliest-finishing compatible activity
- `02_fractionalKnapsack.c` - sort by value/weight ratio, take fractions
- `03_coinChangeGreedy.c` - largest-coin-first; optimal for canonical coin systems only
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
