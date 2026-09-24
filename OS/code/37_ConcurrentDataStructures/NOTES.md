# Concurrent Data Structures

- Complements `21_AdvancedSyncPrimitives` (primitives themselves) and
  `36_LockFreeRingBuffer` (a lock-free structure) with data structures built
  for safe *concurrent* access using coarser (but still non-global) locking
  strategies.

## Lock striping (`01_lockStripedHashMap.c`)

- A single global mutex around a hash map serializes every operation, even
  ones touching completely unrelated keys - the map's actual throughput
  under concurrency is then no better than a single-threaded map.
- Lock striping splits the bucket array into `NUM_STRIPES` groups, each
  guarded by its own mutex: `stripe = hash(key) % NUM_STRIPES`. Two
  operations only contend if their keys land in the same stripe, so the map
  scales with the number of stripes (up to the point of hash collisions
  concentrating traffic in one stripe).
- This is a middle ground between "one global mutex" (simple, no
  concurrency) and true lock-free structures like
  `36_LockFreeRingBuffer/02_mpmcRingBuffer.c` (maximum concurrency, no
  blocking, but far more intricate to get right - CAS loops, sequence
  numbers, memory ordering). Fine-grained locking (striping, or even a
  per-bucket/per-node mutex) sits between: simpler to reason about than
  lock-free code, more concurrent than a single mutex.

## Concurrent skip lists (conceptual only - no `02_` file in this folder)

- A skip list is a probabilistically-balanced, sorted linked structure with
  multiple "levels" of forward pointers, giving O(log n) expected search/
  insert/delete without the rebalancing rotations a tree needs.
- That absence of rotations is exactly why skip lists suit concurrent
  ordered maps: a rebalancing tree operation can touch/re-lock a wide swath
  of the structure, while a skip list insert/delete only needs to link in
  (or unlink) a node at each level it appears at - typically implemented
  with per-node locks (or lock-free CAS on the forward pointers, as in Java's
  `ConcurrentSkipListMap`), so concurrent operations on different regions of
  the list rarely conflict.
- Deliberately not implemented here as running code - a correct concurrent
  skip list (safe insert/delete under concurrent traversal, especially
  lock-free) is a substantial undertaking on its own; this section
  documents the concept rather than a full demo, to be honest about what is
  and isn't implemented in this repo.
