# Graph Algorithms

- Traversal and shortest path basics: `01_adjacencyListBFS.c`,
  `02_adjacencyMatrixDFS.c`, `03_dijkstraShortestPath.c`.

## Topological sort (`04_topologicalSort.c`)

- What it solves: ordering vertices of a directed graph so every edge points
  from an earlier vertex to a later one - only possible on a DAG (directed
  acyclic graph). A cycle means two vertices would each have to come before
  the other, so no valid ordering exists.
- Kahn's algorithm: repeatedly remove a vertex with in-degree 0 (no remaining
  prerequisites), decrementing the in-degree of its neighbors. If fewer than
  `numVertices` vertices get processed, a cycle blocked the rest.
- Use cases: dependency ordering - build systems (compile order), package
  managers (install order), course prerequisites (which courses can be taken
  before which), spreadsheet formula evaluation order.

## Kruskal's vs Prim's MST (`05_kruskalMST.c`, `06_primMST.c`)

- Both find a minimum spanning tree (the cheapest set of edges connecting
  all vertices with no cycles) and agree on total weight for the same graph.
- Kruskal's: sort all edges once, then greedily add each edge unless its
  endpoints are already connected (checked with the union-find structure
  from `42_UnionFind/02_unionFindOptimized.c` - `find` before `union`, same
  cycle-detection pattern noted in that folder's NOTES.md). Natural fit for
  an edge-list representation and sparse graphs, since sorting is
  O(E log E) and union-find operations are near-O(1) amortized.
- Prim's: grow a tree from a start vertex, always adding the cheapest edge
  crossing the cut between the tree and the rest of the graph - the same
  "always take the cheapest next option" O(V^2) array scan Dijkstra's
  `minDistanceVertex` uses. Natural fit for an adjacency-matrix
  representation and dense graphs (or O(E log V) with a binary heap on a
  sparse graph).
- Rule of thumb: sparse graph + edge list -> Kruskal's; dense graph +
  adjacency matrix -> Prim's.

## Bellman-Ford vs Dijkstra (`07_bellmanFord.c`)

- Dijkstra (`03_dijkstraShortestPath.c`) greedily finalizes the closest
  unvisited vertex - O(E log V) with a heap, but wrong (or infinite-looping)
  as soon as a negative edge weight can undo an already-finalized distance.
- Bellman-Ford relaxes every edge, V-1 times - O(V*E), slower, but tolerant
  of negative edge weights. A final V-th pass that can still relax an edge
  means a negative-weight cycle is reachable from the source, so shortest
  paths are undefined (reported instead of returned).
- Use Dijkstra whenever all weights are non-negative; use Bellman-Ford when
  negative weights are possible or negative-cycle detection is needed.

## Floyd-Warshall (`08_floydWarshall.c`)

- All-pairs shortest paths via DP: `dist[i][j]` is repeatedly relaxed by
  allowing routes through each vertex `k` in turn. O(V^3) time, O(V^2)
  space, and the code is a simple triple loop.
- Versus running Dijkstra or Bellman-Ford from every vertex: that approach
  is V * O(E log V) (Dijkstra) or V * O(V*E) (Bellman-Ford), which wins for
  sparse graphs where E is much smaller than V^2. Floyd-Warshall wins for
  dense or small graphs where its O(V^3) is competitive and the
  implementation is far simpler, and it works directly with negative edge
  weights (though not negative cycles, which it can detect via a negative
  value on the diagonal).

## More algorithms in this folder
- **DFS applications:** connected components (label by DFS), cycle detection - undirected: an edge to a visited vertex that is not the parent; directed: an edge to a *grey* vertex (still on the recursion path). Entry/exit times classify edges: tree, back (ancestor, means a cycle), forward, cross.
- **BFS applications:** unweighted shortest path (parent array, walk back), grid/maze shortest path, **bipartite** test (2-colour by level, an edge inside one colour = odd cycle), **multi-source BFS** (push all sources at distance 0; rotting oranges, nearest 0 in a matrix), level-by-level processing.
- **SCC:** Kosaraju = DFS finish order, then DFS on the **transpose** in decreasing finish time; Tarjan = one DFS with index/low-link and a stack, pop a component when `low == index`. Both O(V+E). The condensation (components as nodes) is a DAG.
- **Articulation points / bridges:** DFS with `disc` and `low`. Edge u-v (v child) is a **bridge** if `low[v] > disc[u]`; non-root u is a **cut vertex** if some child has `low[v] >= disc[u]`; the root is a cut vertex iff it has 2+ DFS children. Skip the parent edge (use edge ids with multi-edges).
- **Dijkstra with a heap:** O((V+E) log V); push duplicates and skip stale entries (`d > dist[u]`); non-negative weights only; parent pointers rebuild the path. **A\*** = Dijkstra ordered by `g + h` with an admissible (never overestimating) heuristic such as Manhattan distance on a 4-connected grid; consistent `h` means no re-expansion.
- **0-1 BFS:** deque, push_front for weight-0 edges and push_back for weight-1, O(V+E). **SPFA:** queue-based Bellman-Ford, fast on average, O(VE) worst case; counting relaxations per vertex (> V) detects a negative cycle. **Bellman-Ford** runs V-1 passes; a relaxation in pass V proves a reachable negative cycle - step back V times along parents to land on the cycle.
- **Max flow:** Ford-Fulkerson augments along paths in the residual graph; **Edmonds-Karp** picks the shortest path by BFS, O(V E^2). **Dinic:** BFS level graph + blocking-flow DFS with current-arc pointers, O(V^2 E) (O(E sqrt V) on unit networks). Reverse residual edges let a bad choice be undone. **Max-flow = min-cut:** the vertices reachable from the source in the final residual graph are the source side of a minimum cut.
- **Min-cost max-flow:** successive shortest paths (SPFA, because reverse edges carry negative cost; Dijkstra with potentials is faster); solves assignment and transportation problems.

- **Matching:** Kuhn's algorithm tries to give each left vertex a partner by an **augmenting path** (a free right vertex, or one whose owner can move). O(V E). Hopcroft-Karp batches augmentations (O(E sqrt V)). Konig: in a bipartite graph max matching = min vertex cover; max independent set = V - matching. Reducing to max flow (source -> left -> right -> sink) gives the same answer.
- **Euler path/circuit** (every edge once): undirected needs all degrees even (circuit) or exactly two odd (path, start at an odd vertex) plus connectivity of the edge-bearing vertices; directed needs in = out everywhere (circuit) or one +1/-1 pair. **Hierholzer:** follow unused edges until stuck, append the vertex on return, reverse at the end. Contrast **Hamiltonian** (each vertex once): NP-complete.
- **LCA with binary lifting:** `up[k][v]` = 2^k-th ancestor, O(n log n) preprocessing, O(log n) query (lift the deeper node, then jump both while ancestors differ). Distance = depth[a] + depth[b] - 2 depth[lca]. Diameter by two DFS/BFS sweeps (farthest from anywhere, then farthest from that). Alternatives: Euler tour + sparse table (O(1) query), Tarjan offline LCA with union-find.
- **DAG DP:** longest/shortest path in a DAG in O(V+E) by processing a topological order; earliest start = max over predecessors of (their start + duration); the longest chain is the **critical path** (project length). Counting paths in a DAG uses the same order.
- **Grid graphs:** flood fill / islands by DFS or BFS (mark visited in place), union-find for dynamic land additions; 4- vs 8-connectivity; recursion depth can overflow on big grids - use an explicit stack/queue.
- **Topological order variants:** Kahn with a min-heap gives the lexicographically smallest order; fewer than V nodes output means a cycle; process in waves for parallel scheduling.
- **MST variants:** Prim with a heap O(E log V) (best on dense graphs with an array O(V^2)); Boruvka merges the cheapest outgoing edge of every component each round, O(E log V) and trivially parallel. All use the cut/cycle properties; with distinct weights the MST is unique.
- **Johnson:** add a virtual source, run Bellman-Ford for potentials `h`, reweight `w' = w + h[u] - h[v] >= 0`, run Dijkstra from every vertex, restore with `d - h[s] + h[t]`. O(V E log V): better than Floyd-Warshall on sparse graphs with negative edges.
- **NP-hard graph problems:** m-colouring and Hamiltonian cycle by backtracking (prune on conflicts); greedy colouring uses at most max-degree + 1 colours; **Held-Karp TSP** bitmask DP is O(2^n n^2). Exponential but fine for n <= ~20.
- **2-SAT:** each clause (a or b) gives implications NOT a -> b and NOT b -> a; satisfiable iff no variable shares an SCC with its negation; an assignment follows the component order. Linear time, unlike 3-SAT.
- **Girth:** BFS from each vertex, any non-tree edge closes a cycle of length dist[u] + dist[v] + 1; minimum over all roots. **Implicit graphs** (word ladder, puzzle states): generate neighbours on the fly and BFS for the fewest moves.
