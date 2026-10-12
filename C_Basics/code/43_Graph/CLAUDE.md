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
- `09_graphRepresentationsConversion.c` - Convert between edge list, adjacency matrix and adjacency list; degrees, density, transpose and complement.
- `10_dfsApplications.c` - DFS applications: connected components, cycle detection in undirected and directed graphs, entry/exit times and edge classification.
- `11_bfsApplications.c` - BFS applications: shortest path with reconstruction, grid maze, bipartite test, multi-source BFS and level counting.
- `12_floodFillAndIslands.c` - Grid graphs: flood fill, counting islands (DFS), largest island, and counting with union-find.
- `13_wordLadderImplicitGraph.c` - BFS on an implicit graph: word ladder where two words are adjacent if they differ in one letter.
- `14_courseScheduleOrdering.c` - Course-schedule style problems: is there a valid order, one order, and the lexicographically smallest order with a min-heap.
- `15_dijkstraBinaryHeap.c` - Dijkstra with a binary min-heap (O((V+E) log V)) and shortest-path reconstruction through parent pointers.
- `16_zeroOneBfsAndSpfa.c` - 0-1 BFS with a deque for edges of weight 0 or 1, and SPFA (queue-based Bellman-Ford) for graphs with negative edges.
- `17_bellmanFordNegativeCycle.c` - Bellman-Ford that reports a reachable negative cycle and prints the cycle itself by walking parent pointers.
- `18_aStarGrid.c` - A* search on a grid with the Manhattan-distance heuristic; compares nodes expanded with plain Dijkstra/BFS.
- `19_primHeapAndBoruvka.c` - Two more MST algorithms: Prim with a binary heap (O(E log V)) and Boruvka (merge the cheapest edge of every component per round).
- `20_johnsonAllPairs.c` - Johnson's all-pairs shortest paths for sparse graphs with negative edges: Bellman-Ford reweighting, then Dijkstra from every vertex.
- `21_stronglyConnectedComponents.c` - Strongly connected components with Kosaraju (two passes, transpose) and Tarjan (one pass, low-link values).
- `22_articulationPointsAndBridges.c` - Articulation points and bridges with Tarjan's discovery-time/low-link DFS.
- `23_lcaBinaryLifting.c` - Tree algorithms: parent/depth by DFS, lowest common ancestor with binary lifting, distance between nodes and tree diameter.
- `24_dagLongestPathAndCriticalPath.c` - Longest path in a DAG by DP over a topological order: project scheduling, earliest start times and the critical path.
- `25_shortestCycleAndGirth.c` - Girth (length of the shortest cycle) of an undirected graph by running BFS from every vertex.
- `26_eulerianPathHierholzer.c` - Eulerian circuit/path with Hierholzer's algorithm and the degree conditions for existence.
- `27_bipartiteMatching.c` - Maximum bipartite matching with Kuhn's augmenting-path algorithm and Hopcroft-Karp (BFS layering + DFS, O(E sqrt V)) (jobs to workers), plus the matching/vertex-cover relation.
- `28_maxFlowEdmondsKarp.c` - Maximum flow with Edmonds-Karp (Ford-Fulkerson using BFS augmenting paths) and the min cut it implies.
- `29_maxFlowDinic.c` - Dinic's maximum flow: BFS level graph plus blocking-flow DFS with current-arc pointers, O(V^2 E).
- `30_twoSatWithScc.c` - 2-SAT solved with strongly connected components of the implication graph (Tarjan), including the satisfying assignment.
- `31_graphColoringAndHamiltonian.c` - NP-hard graph problems by backtracking: m-colouring, Hamiltonian cycle, plus a greedy colouring heuristic.
- `32_tspBitmaskDp.c` - Travelling salesman by Held-Karp bitmask DP: O(2^n n^2) instead of O(n!), with tour reconstruction.
- `33_minCostMaxFlow.c` - Minimum-cost maximum flow with successive shortest paths (SPFA on the residual graph): assignment and transport problems.
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
