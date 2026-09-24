# Advanced Hashing and Cache-Aware Structures

## Robin Hood hashing (`01_robinHoodHashing.c`)

- An open-addressing variant on top of `34_HashTable/02_hashTableOpenAddressing.c`'s
  plain linear probing: same idea of probing forward from a key's ideal
  slot, but insertion adds one rule - if the entry being inserted has
  already probed *farther* from its ideal slot than the entry currently
  occupying the candidate slot, the two swap ("rob from the rich, give to
  the poor") and probing continues by re-inserting the displaced entry.
- Net effect: probe-length variance stays low across the table. Plain
  linear probing lets long clusters form (some keys land at distance 0,
  others end up many slots away once a cluster grows); Robin Hood keeps the
  probe distances of occupied slots close to uniform, so no single lookup
  is pathologically slow even under a high load factor - `main()`'s
  printed probe distances after inserting 12 keys into a 16-slot table show
  this: several keys sit at distance 0, but the displaced ones settle at
  small, bounded distances (1-3) rather than one key alone absorbing a long
  chain.
- The `probeDistance` field on each slot also lets `robinHoodLookup` stop
  early: since Robin Hood insertion guarantees non-decreasing probe
  distance along a probe sequence, hitting a slot whose stored distance is
  less than the search's current distance proves the key can't be farther
  along, without needing a tombstone-based deletion scheme.

## Cache-oblivious algorithms (conceptual only, not implemented)

- Designed to perform well across *all* levels of the memory hierarchy
  (L1/L2/L3 cache, RAM, disk) simultaneously, without being tuned to any
  one level's specific block/cache-line size - typically achieved via
  recursive divide-and-conquer that keeps splitting the problem until
  sub-problems fit in whatever cache level happens to be examining them,
  at every level, automatically.
- Contrast with cache-aware algorithms, which are explicitly tuned to a
  specific, known cache-line or block size - e.g. B-trees (see
  `40_BTreeAndBPlusTree`) choose their node fan-out to match a disk
  block size or cache line size exactly, trading portability across
  hardware for being optimal on the hardware they were tuned for.

## Judy arrays (conceptual only, not implemented)

- A cache-optimized associative array (like a hash table or balanced tree)
  implemented as a compressed 256-way trie, where the library adaptively
  chooses among several specialized internal node representations (bitmap,
  linear list, or full array) based on how densely populated a given
  subtree region is - dense regions get denser/faster representations,
  sparse regions get compact ones, all to minimize cache misses.
- A "know it exists" interview topic rather than something to implement
  from scratch here - the interesting fact is *that* production hash-table
  alternatives exist which are explicitly designed around cache behavior
  rather than just asymptotic complexity, in the same spirit as the
  cache-aware-vs-cache-oblivious discussion above.
