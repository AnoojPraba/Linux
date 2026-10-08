# 22_ConsensusAndCoordination

Consensus (Paxos/Raft at a conceptual level), leader election and distributed locking, including the Redlock caveat.

## Files
- `NOTES.md` - (43 lines) sections: Why distributed consensus is hard; Paxos and Raft (conceptual level); Leader election; Distributed locking; Interview framing

## How to use this note
- Drill: explain Raft in two minutes (leader, term, quorum commit) and what happens on a partition.
- Raise the Redlock controversy yourself instead of presenting it as settled.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Consensus is hard because of failures, partitions and message loss/reordering; majority quorum commits a value.
- Raft is the understandable Paxos alternative used by etcd, Consul and CockroachDB.
- Leader election: heartbeats plus a timeout triggers a new election; same mechanism as leader-based replication.
- Distributed locks via ZooKeeper/etcd or Redis Redlock (debated: clock and failure-mode assumptions); use fencing tokens.
- Consensus systems are CP: they give up availability on the minority side of a partition.

## Related
- `../07_DatabaseShardingAndReplication`
- `../08_CAPTheoremAndConsistencyModels`
- `../32_CausalityVectorClocksAndCRDTs`
- `../29_DistributedTransactions2PCSagaOutbox`
- `../20_ServiceDiscoveryAndAPIGateway`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
