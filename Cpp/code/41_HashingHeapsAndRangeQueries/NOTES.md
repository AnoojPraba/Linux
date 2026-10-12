# HashingHeapsAndRangeQueries

- **Chaining** (what `unordered_map` does): a list per bucket; stable references, poor cache behaviour. **Open addressing:** entries live in the array, probe on collision (linear = cache friendly but clusters; quadratic/double hashing spread). Keep load factor < ~0.7, grow by doubling and re-insert everything.
- **Deletion needs tombstones** in open addressing: emptying a slot breaks later probe sequences. `find` continues past a tombstone; `insert` may reuse it after checking the key is absent.
- Bad or attacker-chosen hashes make everything O(n) (hash flooding).
- **Heap** = complete binary tree in an array (children `2i+1`, `2i+2`); push sifts up, pop sifts down, O(log n); `make_heap` is O(n); heap sort O(n log n) in place, not stable. `priority_queue` is a max-heap by default (`std::greater<>` for min); no decrease-key/erase.
- **K-way merge:** min-heap of the head of each list, O(N log k), O(k) memory (external merge sort). **Top-k:** min-heap of size k, O(n log k).
- **Fenwick:** `t[i]` covers `lowbit(i) = i & -i` elements; query/update walk by adding or removing the lowest set bit; needs an invertible operation (sum, xor). **Segment tree:** each node summarizes a range, any associative op, O(log n) query/update; lazy propagation for range updates. Static data -> prefix sums or sparse table.
