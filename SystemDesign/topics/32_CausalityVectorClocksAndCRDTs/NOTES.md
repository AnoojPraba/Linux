# Causality, Vector Clocks and CRDTs

Consistency models: `08_CAPTheoremAndConsistencyModels`; replication:
`07_DatabaseShardingAndReplication`; consensus: `22_...`.

## Why physical clocks are not enough
- Clocks drift and skew (NTP: ms; bad hardware: more); leap seconds; VM pauses.
  "Last write wins by wall clock" silently loses writes when clocks disagree.
- In an asynchronous network there is no global "now". What matters is the
  **happens-before** relation (Lamport): A -> B if A and B are on one process in
  that order, or A is a send and B its receive, or transitively. Events not
  ordered by it are **concurrent**.

## Logical clocks
- **Lamport clock:** one counter per node; increment on each event; stamp messages;
  on receive `c = max(local, msg) + 1`. If A -> B then L(A) < L(B), but NOT the
  converse - it cannot detect concurrency. Break ties with node id for a total order
  (used for ordering in replicated logs, mutual exclusion).
- **Vector clock:** a vector of counters, one per node. Event: increment own entry;
  send: attach vector; receive: element-wise max then increment own. Compare:
  `V1 <= V2` (all entries <=) means V1 happened-before V2; if neither dominates,
  the events are **concurrent** -> a genuine conflict. Cost: O(nodes) size; prune
  or use dotted version vectors / interval tree clocks for dynamic membership.
- **Version vectors** (per object, per replica) are what Dynamo/Riak use to detect
  conflicting siblings; the application or a merge function resolves them.
- **Hybrid Logical Clocks (HLC):** physical time + logical counter - monotonic,
  close to wall time, compact (CockroachDB, YugabyteDB, MongoDB).
- **TrueTime (Spanner):** GPS/atomic-clock bound `[earliest, latest]`; commit-wait
  until uncertainty passes gives external consistency (linearizable) transactions.
- **Snowflake-style ids:** timestamp + node id + sequence - roughly time-ordered
  unique ids without coordination (clock regressions are a hazard).

## Conflict handling strategies
| Strategy | Notes |
|---|---|
| Last-writer-wins (LWW) | simple; loses concurrent updates; needs good clocks/HLC; Cassandra default |
| Keep siblings, merge in app | Dynamo/Riak with vector clocks (shopping-cart union) |
| Operational transformation (OT) | collaborative editing (Google Docs): transform concurrent ops; needs a central server, complex |
| **CRDTs** | data types whose merge is mathematically conflict-free |
| Consensus / single leader | avoid conflicts by serializing (costs availability/latency) |

## CRDTs (Conflict-free Replicated Data Types)
- Replicas update locally, exchange state or operations, and **converge without
  coordination** (strong eventual consistency), tolerating partitions (AP in CAP).
- **State-based (CvRDT):** states form a join-semilattice; merge = least upper bound
  that is **commutative, associative, idempotent** -> order and duplication of
  messages don't matter. **Op-based (CmRDT):** ops are commutative and delivered
  exactly once/causally.
- Examples:
  - **G-Counter:** per-replica counts; value = sum; merge = element-wise max.
  - **PN-Counter:** two G-Counters (increments and decrements).
  - **G-Set / 2P-Set / OR-Set (observed-remove):** OR-Set tags each add with a unique id;
    remove deletes only observed tags, so a concurrent add survives ("add wins").
  - **LWW-Register / MV-Register:** latest timestamp wins / keep concurrent values.
  - **Sequence CRDTs (RGA, Logoot, Yjs, Automerge):** collaborative text with unique
    ordered position ids; used in local-first/offline editors.
- Costs: metadata growth (tombstones, per-replica entries) needing garbage collection,
  restricted semantics (no arbitrary invariants like "balance >= 0" without
  coordination), and merge semantics that may surprise users.
- Used in: Redis Enterprise CRDB, Riak data types, Cosmos DB multi-master, Figma
  multiplayer, Apple Notes, SoundCloud, distributed counters/likes/presence.

## Anti-entropy and propagation
**Gossip** (epidemic dissemination), **read repair**, **Merkle trees** to find
differing ranges cheaply, **hinted handoff** for temporarily down replicas;
**delta-state CRDTs** ship only changes.

## Senior interviewer Q&A
**Q: Why can't you just use timestamps to order events?**
A: Clock skew and drift mean two nodes can disagree on order, and concurrent
events have no true order at all; LWW silently drops one concurrent update.
Logical clocks capture causality; TrueTime/HLC bound or tame physical time.

**Q: Lamport vs vector clocks?**
A: Lamport gives a consistent total order but can't say whether two events are
concurrent. Vector clocks characterize causality exactly (A -> B iff V(A) < V(B)),
detecting concurrency at O(N) space.

**Q: How does Dynamo-style conflict resolution work?**
A: Each write carries a version vector; on conflicting concurrent writes the store
keeps both versions (siblings), returns them on read, and the client/app merges
and writes back a version that dominates both (read repair).

**Q: Explain a G-Counter. Why does merge = max work?**
A: Each replica increments only its own slot, so slots only grow; taking the max
per slot gives the latest known value of each replica, and sum is the total. Max is
commutative/associative/idempotent so merges can be repeated or reordered.

**Q: How would you design a shopping cart / "likes" counter / collaborative
document across regions?**
A: Cart: OR-Set (add wins, removes of observed items), or a sibling-merge. Likes:
PN-Counter or per-user set to avoid double-counting. Document: sequence CRDT
(Yjs/Automerge) or OT with a central sequencer; accept metadata overhead, add GC
and snapshots.

**Q: What can't CRDTs do?**
A: Enforce global invariants (unique usernames, non-negative balance, inventory
limits) without coordination - use consensus/escrow/reservations for those.
