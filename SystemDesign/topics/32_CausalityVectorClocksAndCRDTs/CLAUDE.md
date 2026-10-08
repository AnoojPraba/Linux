# 32_CausalityVectorClocksAndCRDTs

Ordering without a global clock: happens-before, Lamport and vector clocks, HLC/TrueTime, conflict-handling strategies and CRDTs, with a senior Q&A.

## Files
- `NOTES.md` - (98 lines) sections: Why physical clocks are not enough; Logical clocks; Conflict handling strategies; CRDTs (Conflict-free Replicated Data Types); Anti-entropy and propagation; Senior interviewer Q&A

## How to use this note
- Drill: show with a three-node example why Lamport clocks cannot detect concurrency but vector clocks can; then build a G-Counter and OR-Set on the whiteboard.
- Rehearse the "Senior interviewer Q&A": timestamps vs causality, Lamport vs vector, Dynamo conflicts, G-Counter, designing cart/likes/document, what CRDTs cannot do.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Physical clocks drift; last-write-wins by wall clock silently loses concurrent writes.
- Lamport: L(A) < L(B) if A -> B but not the converse; vector clocks characterise causality and detect concurrency at O(nodes) space; version vectors drive Dynamo/Riak siblings.
- HLC (physical + logical counter) and TrueTime (bounded uncertainty, commit-wait) tame physical time; Snowflake ids need clock-regression care.
- Strategies: LWW (simple, lossy), keep siblings and merge, OT (central server), CRDTs (conflict-free merge), or serialise via consensus.
- CRDTs: merge must be commutative, associative and idempotent; G-Counter (per-replica max, sum), PN-Counter, OR-Set (add wins), sequence CRDTs for text; cost is metadata growth, and they cannot enforce global invariants (unique names, non-negative balance).
- Anti-entropy: gossip, read repair, Merkle trees, hinted handoff, delta-state CRDTs.

## Related
- `../08_CAPTheoremAndConsistencyModels`
- `../07_DatabaseShardingAndReplication`
- `../22_ConsensusAndCoordination`
- `../15_DesignCaseStudyDistributedCache`
- `../33_DesignCaseStudyNewsFeed`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
