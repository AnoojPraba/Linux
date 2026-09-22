A heap is a tree-shaped data structure, almost always stored in a plain array,
that keeps one ordering guarantee at all times:

*   Min-heap: every parent's value <= both its children's values.
*   Max-heap: every parent's value >= both its children's values.

That's the *only* rule - unlike a BST, siblings and cousins have no required
order relative to each other. This weaker guarantee is exactly what makes a
heap cheap to maintain: insert and remove are both O(log n), and finding the
min (or max) is O(1) - it's always the root.

Why an array instead of pointers?

For index i (0-based):
*   left child  = 2*i + 1
*   right child = 2*i + 2
*   parent      = (i - 1) / 2

No struct, no next/left/right pointers needed - the array index itself
encodes the whole tree shape, as long as the heap stays "complete" (every
level full except possibly the last, filled left to right). That's also why
you can't binary-search a heap - it's shaped like a tree, but not ordered
like a BST.

The two core operations:

*   push (insert): put the new value at the end of the array, then "bubble
    up" - swap with its parent as long as it's smaller (min-heap) than that
    parent.
*   pop (extract min/max): the root is always the answer. Move the last
    array element into the root slot, shrink the array by one, then "bubble
    down" - swap with the smaller child as long as this node is bigger than
    it.

What heaps are actually used for:

*   Priority queues - "give me the most urgent item next", e.g. an OS
    scheduler, or an event-driven simulation's next event.
*   Heap sort - build a heap, then repeatedly pop the root; O(n log n),
    in-place, no extra memory (unlike merge sort).
*   Graph algorithms - Dijkstra's shortest path and Prim's minimum spanning
    tree both repeatedly ask "which unvisited node is closest/cheapest right
    now?" - exactly what a min-heap answers in O(log n) instead of the O(n)
    linear scan a naive implementation would use.

See the runnable examples: `code/31_Heap/01_minHeap.c` (push/pop) and
`code/31_Heap/02_heapSort.c` (heap sort). `code/32_Graph/03_dijkstraShortestPath.c`
notes in a comment where a real implementation would swap its O(V) linear
scan for a min-heap.
