A graph is just a set of vertices (nodes) and edges (connections between
them). That's it - no required parent/child hierarchy like a tree, and
unlike a tree, a graph can have cycles (a path that loops back on itself)
and a vertex can have any number of connections in any direction.

Every tree is technically a graph (a connected graph with no cycles and
exactly one path between any two vertices) - but not every graph is a tree.
The binary search trees in `code/30_BinaryTree/` are the special case;
`code/32_Graph/` covers the general one.

Directed vs. undirected:
*   Undirected - an edge between A and B means you can go A -> B and
    B -> A (e.g. a friendship, a two-way road).
*   Directed - an edge only goes one way, A -> B does not imply B -> A
    (e.g. a follows relationship, a one-way street).

Weighted vs. unweighted:
*   Unweighted - every edge is equally "expensive" to traverse.
*   Weighted - each edge carries a cost (distance, time, price); this is
    what makes "shortest path" a meaningful question rather than just
    "fewest hops".

Two common ways to represent a graph in code:

*   Adjacency list - an array where index i holds a linked list of i's
    neighbors. Memory proportional to the actual number of edges; checking
    "is there an edge from A to B" costs O(degree of A).
    See `code/32_Graph/01_adjacencyListBFS.c`.
*   Adjacency matrix - an N x N array where matrix[i][j] is 1 (or the edge
    weight) if an edge exists. Always O(V^2) memory regardless of edge
    count, but checking "is there an edge" is O(1).
    See `code/32_Graph/02_adjacencyMatrixDFS.c`.

The two fundamental traversals - both need a "visited" marker, because a
graph's cycles mean naive recursion/looping could revisit the same vertex
forever, which a tree traversal never has to worry about:

*   BFS (breadth-first search) - visit all neighbors at the current
    distance before going further out, using an explicit queue. Answers
    "what's the fewest-hops path?" on an unweighted graph.
*   DFS (depth-first search) - go as deep as possible down one path before
    backtracking, usually via recursion (the call stack acts as the
    implicit stack DFS needs).

What graphs are actually used for:

*   Shortest path - Dijkstra's algorithm (weighted, non-negative edges) -
    see `code/32_Graph/03_dijkstraShortestPath.c`, which leans on the same
    "grab the cheapest next option" idea as the min-heap in
    `Notes/12_Heap.c`.
*   Modeling networks, dependencies, and relationships - road maps, social
    networks, build/task dependency graphs (where a cycle means "circular
    dependency" - a bug you want DFS to help you detect), routing tables.
