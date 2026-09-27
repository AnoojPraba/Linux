# Consensus and Coordination

## Why distributed consensus is hard

- Multiple nodes need to agree on a single value/decision despite node
  failures and network partitions/message loss/reordering (see
  `08_CAPTheoremAndConsistencyModels`).

## Paxos and Raft (conceptual level)

- Shape of the solution, not full algorithm detail: a leader is elected,
  the leader proposes values, followers acknowledge, and a value is
  committed once a majority (quorum) of nodes acknowledge it.
- Raft was explicitly designed to be more understandable than Paxos while
  solving the same problem, and is what most modern systems actually
  implement (etcd, Consul, CockroachDB).

## Leader election

- How a cluster picks a leader, and re-elects one if it fails: nodes send
  heartbeats; if a follower stops hearing from the leader within a
  timeout, it triggers a new election.
- Same underlying mechanism used for leader-based replication in
  `07_DatabaseShardingAndReplication`.

## Distributed locking

- Use a coordination service (ZooKeeper, etcd) or a data store (Redis
  with the Redlock algorithm) to ensure only one node performs a given
  action at a time (e.g. a single leader running a scheduled job across a
  fleet).
- Note: Redlock is debated/controversial in distributed systems circles
  (concerns around clock assumptions and failure modes) - worth
  mentioning the nuance in an interview rather than presenting it as a
  settled solution.

## Interview framing

- Consensus underpins reliable leader election and replication
  (`07_DatabaseShardingAndReplication`) and is the CP side of the CAP
  tradeoff (`08_CAPTheoremAndConsistencyModels`) - a consensus-based
  system sacrifices availability during a partition to guarantee a single
  agreed-upon value.
