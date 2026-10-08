# 15_DesignCaseStudyDistributedCache

Case study: distributed cache cluster (consistent hashing with virtual nodes, replication, eviction, Dynamo-style failure handling, cache-aside vs transparent cache).

## Files
- `NOTES.md` - (54 lines) sections: Node assignment: consistent hashing; Replication for availability; Eviction policies; Handling node failure (Dynamo-style concepts); Cache-aside vs a transparent caching layer

## How to use this note
- Whiteboard a Redis/Memcached-like cluster: ring, replicas, eviction, node failure, and how clients find keys.
- Tie each choice back to CAP: a cache usually prefers availability.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Consistent hashing remaps only keys between the changed node and its neighbour; virtual nodes even out load.
- Replicate each key to N nodes along the ring (preference list) so one failure loses nothing; AP-leaning.
- LRU is the cheap default; LFU wins when frequency predicts reuse but costs more bookkeeping.
- Hinted handoff keeps writes available during transient failure; read repair heals stale replicas during reads.
- Cache-aside (app-managed) vs transparent cache-in-front-of-DB (simpler apps, but a more critical dependency with its own consistency duties).

## Related
- `../02_LoadBalancing`
- `../03_CachingStrategies`
- `../08_CAPTheoremAndConsistencyModels`
- `../26_DesignCaseStudyConcurrentInMemoryKeyValueStore`
- `../../../C_Basics/code/37_LRUCache`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
