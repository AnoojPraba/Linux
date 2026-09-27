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
