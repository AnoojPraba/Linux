# Design Case Study: Distributed Cache

Designing something like a distributed Redis/Memcached cluster - draws
directly on `../02_LoadBalancing` (consistent hashing) and
`../08_CAPTheoremAndConsistencyModels` (AP-leaning design choices).

## Node assignment: consistent hashing

- Nodes and keys are placed on a hash ring; a key is owned by the next
  node clockwise. Adding/removing a node only remaps the keys between it
  and its neighbor, avoiding a full-cluster reshuffle (see
  `../02_LoadBalancing` for why this matters).
- Virtual nodes (multiple ring positions per physical node) smooth out
  uneven load distribution that plain consistent hashing can otherwise
  produce with a small number of nodes.

## Replication for availability

- Each key is replicated to N nodes (its "preference list" walking forward
  around the ring from its primary owner), so a single node failure doesn't
  lose the data or make it unavailable.
- Ties into CAP: a distributed cache typically favors availability over
  strict consistency (AP) - it's a cache, so serving slightly stale data
  is usually preferable to refusing to serve at all.

## Eviction policies

- **LRU (Least Recently Used)**: evict the entry that hasn't been accessed
  in the longest time - good general-purpose default, cheap to approximate.
- **LFU (Least Frequently Used)**: evict the entry accessed least often
  overall - better than LRU when access frequency, not recency, predicts
  future use, but more bookkeeping overhead.

## Handling node failure (Dynamo-style concepts)

- **Hinted handoff**: if a node that should receive a write is temporarily
  down, another node accepts the write on its behalf and hands it off once
  the original node recovers - keeps writes available during transient
  failures.
- **Read repair**: when a read notices replicas disagree (stale replica
  found during a quorum read), the coordinator writes the correct value
  back to the stale replica(s) as a side effect of serving the read,
  gradually healing inconsistency without a separate repair process.

## Cache-aside vs a transparent caching layer

- **Cache-aside** (application-managed, see `../03_CachingStrategies`): the
  application explicitly checks the cache, falls back to the DB on a miss,
  and populates the cache itself.
- **Transparent cache-in-front-of-DB**: the cache layer itself proxies
  reads/writes to the underlying DB on a miss, so the application only ever
  talks to the cache. Simpler for application code, but adds a layer that
  must correctly handle consistency between itself and the DB, and becomes
  a more critical dependency (harder to bypass if it misbehaves).
