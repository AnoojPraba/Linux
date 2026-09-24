# Union-Find (Disjoint Set Union)

- What it solves: tracking a dynamic partition of elements into disjoint
  sets, supporting two operations efficiently - `find` (which set does an
  element belong to?) and `union` (merge two sets).
- Use in Kruskal's MST: process edges in increasing weight order, and add an
  edge only if its two endpoints are in different sets (`find` differs);
  after adding, `union` the two sets. This avoids ever forming a cycle
  without needing a full graph traversal per edge. See `43_Graph` for the
  graph representations this operates on.
- Cycle detection: in an undirected graph, adding an edge between two
  vertices already in the same set means that edge would close a cycle -
  the same `find`-before-`union` check Kruskal's uses.
- Union by rank/size: naively always attaching one root under the other can
  build a long chain (degenerating `find` toward O(n)). Union by rank
  attaches the shorter tree under the taller one's root (tracking an upper
  bound on height); union by size does the same using subtree element count
  instead. Either keeps trees shallow.
- Path compression: during `find`, every node visited on the way to the
  root is repointed directly at the root. Future `find` calls on those
  nodes become O(1). Combined with union by rank/size, this gives amortized
  near-O(1) (inverse-Ackermann) operations - the naive version without
  either optimization can degrade toward O(n) per operation.
