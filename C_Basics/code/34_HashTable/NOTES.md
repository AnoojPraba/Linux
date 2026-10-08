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

## Senior interviewer Q&A
**Q: Implement a hash table from scratch - what decisions do you make?**
A: Hash function (good avalanche, e.g. FNV-1a/xxHash/SipHash for untrusted keys),
power-of-two capacity (mask instead of `%`) with a mixing step, collision strategy
(chaining vs open addressing), max load factor (~0.7 for linear probing, ~1 for
chaining), growth policy (double and rehash), and delete handling (tombstones or
backward-shift deletion). Define ownership of keys/values and handle failed
allocation during resize.

**Q: Why is resizing O(n) yet insertion is amortized O(1)?**
A: Doubling means each element is moved O(1) times on average over a sequence of
inserts (geometric series). A single insert can spike latency - problematic for
real-time/latency-sensitive paths; use **incremental rehashing** (Redis keeps two
tables and migrates a few buckets per operation).

**Q: Linear probing vs quadratic vs double hashing vs Robin Hood?**
A: Linear probing: best cache behavior, suffers primary clustering. Quadratic: less
clustering, can miss slots if capacity isn't prime/power-of-two with triangular
numbers. Double hashing: least clustering, worse locality. Robin Hood (see
`../36_AdvancedHashingAndCacheAwareStructures`): equalizes probe distances, lowers
variance, enables early termination on lookup misses; backward-shift deletion removes
the need for tombstones.

**Q: How do you delete in open addressing without breaking lookups?**
A: Tombstones (mark deleted, keep probing; rehash when too many accumulate) or
backward-shift deletion (move later entries back to fill the hole while preserving
probe order - needs the "ideal bucket" or probe distance).

**Q: How can an attacker abuse a hash table? How do you defend?**
A: Hash-flooding: crafted keys collide into one bucket, turning O(1) into O(n) and
causing DoS (the 2011 web-framework attacks). Defend with a keyed hash (SipHash) with a
random per-process seed, or tree-ified buckets (Java 8 HashMap), and limit
untrusted-key inputs.

**Q: Chaining vs open addressing - which is faster in practice?**
A: Open addressing is typically faster for small keys/values due to locality and no
allocations (Swiss tables/F14/hashbrown use SIMD group probing); chaining tolerates
high load factors and large/expensive-to-move values, gives stable pointers/iterators.

**Q: Design a thread-safe hash map.**
A: Options: one global lock (simple, contended), lock striping per bucket group
(`../../../OS/code/38_ConcurrentDataStructures`), reader-writer locks, per-bucket
lock-free lists, concurrent open addressing with CAS, or read-copy-update for
read-mostly tables; resizing is the hard part (cooperative incremental resize).

**Q: Hash function vs good key equality - what bugs do you see?**
A: Keys that are equal must hash equally (equal objects, same normalization), hashing
mutable keys that change after insertion, hashing pointers vs contents, hashing
padding bytes/uninitialized struct bytes (nondeterministic), float `-0.0`/`NaN`
semantics, and signed/unsigned or endianness differences across machines.

**Q: What load factor would you choose and why?**
A: ~0.5-0.7 for linear probing (expected probes grow like 1/(1-a)^2 for misses),
~0.75-1.0 for chaining; lower when memory is cheap and latency matters, higher for
read-only tables built once.
