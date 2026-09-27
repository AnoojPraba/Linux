# Heaps

## Min-heap vs max-heap use cases

- **Min-heap** - the smallest element is always at the root, so it's the
  right structure whenever you repeatedly need the *smallest* remaining item:
  - Dijkstra's algorithm: the priority queue of "next node to visit" is
    ordered by smallest known distance, and a min-heap gives O(log n)
    extract-min/decrease-key operations.
  - Finding the k smallest elements of a stream/array: keep a min-heap of
    size k (or push everything and pop k times from a full min-heap) so the
    smallest is always cheaply accessible.
- **Max-heap** - the largest element is always at the root, so it's the right
  structure whenever you repeatedly need the *largest* remaining item:
  - Heapsort producing ascending order: build a max-heap, then repeatedly
    swap the root (the current maximum) to the end of the array and shrink
    the heap - this is exactly what `02_heapSort.c` does.
  - Finding the k largest elements of a stream/array: same idea as the
    min-heap case, mirrored - a max-heap keeps the current largest at the
    root for O(log n) extraction.
  - Priority queues where a higher value means higher priority (e.g. task
    schedulers where priority number = importance) - `extractMax` always
    hands back the most important pending item.
