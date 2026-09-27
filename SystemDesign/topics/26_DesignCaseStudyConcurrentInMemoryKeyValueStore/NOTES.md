# Design Case Study: Concurrent In-Memory Key-Value Store

Designing something like Redis - a single-node, in-memory key-value store
that needs to serve many concurrent clients. The interesting twist is that
"concurrent" doesn't have to mean "multi-threaded with locks."

## Requirements clarification (ask before designing)

- What data types are in scope - just string get/set, or also
  lists/sets/hashes/sorted-sets like real Redis?
- What durability is required - a pure cache that can lose everything on
  restart, or does it need persistence (append-only log / snapshotting)?
- Single node, or does clustering/sharding need to be discussed too?
- Expected read:write ratio and dataset size - does the whole dataset fit
  in memory on one box, or does this need to be a distributed design (see
  `../15_DesignCaseStudyDistributedCache` for the multi-node version)?

## Core lookup structure

- A hash table for O(1) average-case key lookup - chaining vs open
  addressing is the standard tradeoff (chaining is simpler and degrades
  more gracefully; open addressing has better cache locality but needs
  careful handling of deletions/tombstones and load-factor growth).

## Concurrency model: this is the crux of the question

- **Naive multi-threaded + global lock**: simple and correct, but
  serializes every operation - defeats the purpose of having multiple
  threads.
- **Lock striping / sharding the keyspace across locks**: better
  parallelism, more implementation complexity, and breaks atomicity of
  multi-key operations unless carefully coordinated (e.g. always locking
  in a consistent order to avoid deadlock).
- **Single-threaded event loop (what real Redis does)**: one thread
  processes all commands sequentially against an event loop (like
  Node.js), avoiding lock overhead and contention entirely. This works
  well because for a KV store the bottleneck is usually network I/O, not
  CPU - a single thread can still saturate a network link while paying
  zero synchronization cost.
- A strong answer articulates *both* directions: why single-threaded is a
  legitimate, even preferred, design here, **and** how to scale past one
  core if needed - shard the keyspace across N independent single-threaded
  instances (this is exactly what Redis Cluster does), rather than trying
  to make one instance internally multi-threaded.

## Eviction policy (for a fixed-memory cache)

- **LRU**: evict the least-recently-used entry - simple, effective
  general-purpose default (see `../15_DesignCaseStudyDistributedCache` for
  the same tradeoff in the distributed setting).
- **LFU**: evict by access frequency rather than recency - better when
  frequency, not recency, predicts future use.
- **TTL-based expiration**: lazy expiration (check TTL on access) combined
  with a periodic active sweep of a random sample of keys - this is what
  Redis actually does, since eagerly checking every key's TTL on every
  clock tick would itself be a CPU cost.

## Persistence (if in scope)

- **Snapshotting**: periodic full dump of the dataset to disk - simple,
  but can lose writes since the last snapshot on a crash.
- **Append-only log (journal)**: every write is logged before/as it's
  applied, replayed on restart for durability - safer per-write cost is
  higher, and the log needs periodic compaction to avoid growing
  unboundedly. This is the same durability-vs-latency tradeoff as a
  database write-ahead log (see `../05_ACIDAndTransactionIsolation`).

## What interviewers are actually listening for

- Not defaulting to "multi-threaded with a lock" as the only concurrency
  model - recognizing the single-threaded-event-loop alternative and
  explaining *why* it's often the more sophisticated answer for an
  I/O-bound service.
- A clear, explicit articulation of the eviction and durability tradeoffs
  rather than picking one option without discussing the alternative.
