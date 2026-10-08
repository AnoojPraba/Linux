# 42_UnionFind

Disjoint-set union: naive version vs union-by-rank with path compression.

## Files
- `01_unionFindNaive.c` - find without compression, plain union
- `02_unionFindOptimized.c` - path compression plus union by rank
- `NOTES.md` - what DSU solves, Kruskal use, cycle detection, union by rank/size, path compression

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_unionFindNaive.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/42_UnionFind/` (git-ignored).

## Key concepts / interview angles
- Naive operations can degenerate to O(n) via long chains.
- Rank/size plus path compression gives amortised inverse-Ackermann (near O(1)).
- Uses: Kruskal MST, undirected cycle detection, connected components, grid/percolation problems.

## Related
- `../43_Graph/05_kruskalMST.c`
- `../43_Graph`
- `../49_SlidingWindowAndTwoPointer`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
