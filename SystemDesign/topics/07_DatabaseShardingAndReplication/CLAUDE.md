# 07_DatabaseShardingAndReplication

Sharding strategies (range, hash, geographic), hot shards, leader-follower replication, replication lag and sync vs async replication.

## Files
- `NOTES.md` - (41 lines) sections: Sharding strategies; Replication

## How to use this note
- Drill: choose a shard key for a given product, state which queries become cross-shard, and describe a resharding plan.
- Explain read-your-own-writes with lagging followers and a mitigation.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Range sharding: range scans easy, hot ranges (newest shard); hash sharding: even load but range queries fan out; consistent hashing limits reshuffling.
- Geographic sharding cuts latency and helps data residency but complicates cross-region queries.
- Hot shard fixes: higher-cardinality key, splitting, caching in front.
- Leader-follower scales reads and gives failover; async replication can lose the latest writes on leader crash, sync adds latency and stalls on a slow follower.

## Related
- `../02_LoadBalancing`
- `../08_CAPTheoremAndConsistencyModels`
- `../22_ConsensusAndCoordination`
- `../15_DesignCaseStudyDistributedCache`
- `../29_DistributedTransactions2PCSagaOutbox`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
