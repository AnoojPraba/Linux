# 08_CAPTheoremAndConsistencyModels

CAP theorem (really CP vs AP under partitions), strong vs eventual consistency and quorum reads/writes (W + R > N).

## Files
- `NOTES.md` - (40 lines) sections: CAP theorem; Strong vs eventual consistency; Quorum reads/writes

## How to use this note
- Drill: classify a few systems (RDBMS, ZooKeeper/etcd, Cassandra, DynamoDB) as CP or AP and explain the partition behaviour.
- Compute quorum settings for N=3 and N=5 and what each tolerates.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Partitions are a given, so the real choice is CP (refuse requests on the minority side) vs AP (serve possibly stale data).
- Strong consistency is simpler for applications but costs latency and availability; eventual is cheaper and common in AP systems.
- W + R > N makes read and write quorums overlap, giving up-to-date reads without touching all nodes.
- Lower W/R means faster and more available but weaker consistency.
- CAP says nothing about latency; PACELC extends it (mention if asked).

## Related
- `../07_DatabaseShardingAndReplication`
- `../22_ConsensusAndCoordination`
- `../32_CausalityVectorClocksAndCRDTs`
- `../05_ACIDAndTransactionIsolation`
- `../24_IdempotencyInDistributedSystems`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
