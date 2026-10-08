# 41_Heap

Binary heaps as arrays: min-heap, max-heap and heap sort, with use cases (priority queues, k-largest/smallest, Dijkstra).

## Files
- `01_minHeap.c` - array min-heap with sift-up insert and extract-min
- `02_heapSort.c` - max-heap build then repeatedly swap root to the end
- `03_maxHeap.c` - array max-heap insert/extractMax
- `NOTES.md` - min-heap vs max-heap use cases

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_minHeap.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/41_Heap/` (git-ignored).

## Key concepts / interview angles
- Array layout: children of `i` at `2i+1`, `2i+2`; parent at `(i-1)/2`.
- Insert O(log n), extract O(log n), peek O(1), build-heap O(n).
- Heap sort ascending uses a max-heap; k largest uses a size-k min-heap (counter-intuitive but standard).
- Heaps are not sorted and offer no O(log n) arbitrary delete/decrease-key without an index map.

## Related
- `../12_Sorting/03_heapSort.c`
- `../43_Graph/03_dijkstraShortestPath.c`
- `../../../Cpp/code/18_STLContainers` - std::priority_queue

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
