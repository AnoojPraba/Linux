# 43_Graph

Graph representations and core algorithms: BFS, DFS, Dijkstra, topological sort, Kruskal/Prim MST, Bellman-Ford and Floyd-Warshall.

## Files
- `01_adjacencyListBFS.c` - adjacency list (linked lists) with BFS on an undirected graph
- `02_adjacencyMatrixDFS.c` - adjacency matrix (O(V^2) memory, O(1) edge check) with DFS
- `03_dijkstraShortestPath.c` - single-source shortest path with an O(V^2) scan for the min vertex
- `04_topologicalSort.c` - directed dependency graph ordering (indegree/DFS based)
- `05_kruskalMST.c` - sort edges and union-find with rank and path compression
- `06_primMST.c` - grow the tree by the cheapest crossing edge (O(V^2) scan)
- `07_bellmanFord.c` - relax all edges V-1 times; demo includes a negative edge but no negative cycle
- `08_floydWarshall.c` - all-pairs shortest paths by DP over intermediate vertices
- `NOTES.md` - topological sort, Kruskal vs Prim, Bellman-Ford vs Dijkstra, Floyd-Warshall

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_adjacencyListBFS.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/43_Graph/` (git-ignored).

## Key concepts / interview angles
- List vs matrix: sparse graphs favour lists; dense or O(1) edge queries favour matrices.
- Dijkstra needs non-negative weights; Bellman-Ford handles negative edges and detects negative cycles in O(VE).
- Topological sort exists only for DAGs; used for build/dependency ordering.
- Kruskal (sort edges + DSU) vs Prim (grow from a vertex); both O(E log V) with the right structures.
- The Dijkstra/Prim here use O(V^2) scans, not a heap; mention a heap version gives O((V+E) log V).

## Related
- `../42_UnionFind`
- `../41_Heap`
- `../32_StackAndQueue`
- `../46_DynamicProgramming`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
