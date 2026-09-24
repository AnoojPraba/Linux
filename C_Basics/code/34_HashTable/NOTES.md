# Hash Table

- Load factor (`n / TABLE_SIZE`) measures how full the table is; as it grows,
  chains get longer (chaining) or probe sequences get longer (open
  addressing), degrading toward O(n) lookups.
- Resizing/rehashing: once load factor crosses a threshold (commonly ~0.7),
  allocate a bigger table (typically double) and reinsert every existing
  entry, since the bucket/probe index depends on the table size.
- Chaining vs open addressing:
  - Chaining: simple, degrades gracefully, table never "fills up" (buckets
    just grow), but each entry needs extra memory for a linked list node/
    pointer, and cache locality is worse (pointer chasing).
  - Open addressing: all entries live in one contiguous array (better cache
    locality, no per-entry allocation), but the table can fill up, clustering
    can hurt performance, and deletion needs tombstones (see below).
- Average O(1) vs worst-case O(n): with a good hash function and reasonable
  load factor, lookups are O(1) average. Worst case is O(n) if many/all keys
  collide into the same bucket (chaining) or probe sequence (open
  addressing) - e.g. a degenerate/adversarial hash function.
- Why a good hash function matters: it must spread keys uniformly across
  buckets. A poor hash (e.g. summing character codes, which collides for
  anagrams) causes clustering and pushes real-world performance toward the
  O(n) worst case even with low load factor.
- Tombstone gotcha (open addressing delete): clearing a deleted slot back to
  "empty" would break probing for any other key whose probe sequence passed
  through that slot - lookups would stop early and report false negatives.
  A tombstone marks "occupied once, keep probing past me" without matching
  any key.
