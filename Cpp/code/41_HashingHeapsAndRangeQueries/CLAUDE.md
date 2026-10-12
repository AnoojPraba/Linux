# 41_HashingHeapsAndRangeQueries

Hash tables (chaining and open addressing with tombstones), heaps (k-way merge, top-k) and range-query trees (Fenwick, segment tree).

## Files
- `01_hashTableChaining.cpp` - vector of std::list buckets with std::hash; the idea behind unordered_map.
- `02_openAddressingTable.cpp` - Linear probing, tombstones, load-factor 0.7 resizing, heterogeneous operations.
- `03_heapKwayMergeTopK.cpp` - Hand-written binary heap, k-way merge with priority_queue, top-k of a stream, make_heap/pop_heap.
- `04_fenwickAndSegmentTree.cpp` - Fenwick tree (prefix sums with point updates) and iterative min segment tree.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `g++ -std=c++20 -Wall -Wextra -pthread 01_hashTableChaining.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/41_HashingHeapsAndRangeQueries/` (git-ignored).

## Key concepts / interview angles
- **Chaining** (what `unordered_map` does): a list per bucket; stable references, poor cache behaviour. **Open addressing:** entries live in the array, probe on collision (linear = cache friendly but clusters; quadratic/double hashing spread). Keep load factor < ~0.7, grow by doubling and re-insert everything.
- **Deletion needs tombstones** in open addressing: emptying a slot breaks later probe sequences. `find` continues past a tombstone; `insert` may reuse it after checking the key is absent.
- Bad or attacker-chosen hashes make everything O(n) (hash flooding).
- **Heap** = complete binary tree in an array (children `2i+1`, `2i+2`); push sifts up, pop sifts down, O(log n); `make_heap` is O(n); heap sort O(n log n) in place, not stable. `priority_queue` is a max-heap by default (`std::greater<>` for min); no decrease-key/erase.
- **K-way merge:** min-heap of the head of each list, O(N log k), O(k) memory (external merge sort). **Top-k:** min-heap of size k, O(n log k).

## Gotchas
- Re-hashing invalidates iterators and references of open-addressing tables.
- Fenwick indexes are 1-based internally; the wrapper converts.
