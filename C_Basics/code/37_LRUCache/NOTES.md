# LRU Cache (O(1) get/put)

One of the most commonly asked "combine two data structures" interview
questions: implement a fixed-capacity cache with O(1) `get(key)` and
`put(key, value)`, evicting the least-recently-used entry on overflow.

## Why neither structure alone is enough

- **Array alone**: O(1) access by index, but O(n) to find an entry by key
  (linear scan), and O(n) to reorder (shifting elements to mark an entry as
  most recently used).
- **Linked list alone** (even doubly-linked, with recency order maintained by
  position): O(1) to move a node once you have a pointer to it, but O(n) to
  *find* that node by key in the first place, since a list has no random
  access by key.
- **Hash map alone**: O(1) find by key, but a hash map has no inherent
  ordering, so there is no O(1) way to know which entry is least recently
  used.

## The combination

- A **doubly-linked list (DLL)** maintains recency order: most-recently-used
  at the head, least-recently-used at the tail. Moving a node to the head
  (on access) and evicting the tail (on overflow) are both O(1) once you
  have a pointer to the node - no scanning required.
- A **hash map** stores `key -> DLL node pointer`, giving O(1) lookup of the
  node for a given key.
- Each structure is used for exactly what it's good at: the hash map for
  O(1) *lookup*, the DLL for O(1) *reordering and eviction*. Neither
  structure has to do the other's job.
- `get(key)`: hash map lookup -> O(1); detach + re-insert at DLL head -> O(1).
- `put(key, value)`: hash map lookup to check for update -> O(1); if new,
  evict DLL tail (and remove its hash map entry) if at capacity, then insert
  at DLL head and add to the hash map -> all O(1).

See `33_LinkedList` and `34_HashTable` for the two
building blocks this folder combines.

## Thread safety

The cache above is not thread-safe. A concurrent version needs additional
locking, with a tradeoff:
- **Single mutex around the whole structure**: simple to reason about and
  correct, but serializes every `get`/`put` call - no concurrency benefit
  under contention.
- **Lock striping / sharding by key hash**: split the cache into N
  independently-locked shards (e.g. by `hash(key) % N`), so operations on
  different shards can proceed in parallel. This trades away *global*
  recency ordering - LRU eviction only happens within a shard, not across
  the whole cache - for better throughput. This is the same sharding idea
  used by concurrent hash map implementations.

This repo doesn't yet have a dedicated thread-safe LRU wrapper example;
`../../Cpp/code/24_Concurrency` covers general mutex/lock-guard patterns
(e.g. `02_mutexAndLockGuard.cpp`) that would be the building block for a
single-mutex version if one were added later.
