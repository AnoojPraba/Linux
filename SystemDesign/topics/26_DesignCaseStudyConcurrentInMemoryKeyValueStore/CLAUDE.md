# 26_DesignCaseStudyConcurrentInMemoryKeyValueStore

Case study: single-node Redis-like in-memory key-value store; the crux is the concurrency model (global lock vs lock striping vs single-threaded event loop), plus eviction and persistence.

## Files
- `NOTES.md` - (75 lines) sections: Requirements clarification (ask before designing); Core lookup structure; Concurrency model: this is the crux of the question; Eviction policy (for a fixed-memory cache); Persistence (if in scope); What interviewers are actually listening for

## How to use this note
- Whiteboard: hash table choice, three concurrency models, eviction, persistence; argue why single-threaded is legitimate and how to scale beyond one core (shard into N instances).
- Offer both directions; picking only "multi-threaded with a lock" is the weak answer.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Hash table: chaining degrades gracefully; open addressing is cache friendly but needs tombstones and load-factor management.
- Global lock is simple but serialises; lock striping gives parallelism but breaks multi-key atomicity unless lock order is consistent.
- Single-threaded event loop (Redis): no lock overhead because the bottleneck is network I/O; scale out by sharding the keyspace across instances (Redis Cluster).
- Eviction: LRU default, LFU when frequency predicts reuse; TTL via lazy expiry plus periodic random-sample sweep.
- Persistence: snapshots (lose writes since last) vs append-only log (safer, needs compaction), the same trade-off as a WAL.

## Related
- `../15_DesignCaseStudyDistributedCache`
- `../03_CachingStrategies`
- `../05_ACIDAndTransactionIsolation`
- `../../../C_Basics/code/34_HashTable`
- `../../../OS/code/38_ConcurrentDataStructures`
- `../../../OS/code/67_EpollInDepth`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
