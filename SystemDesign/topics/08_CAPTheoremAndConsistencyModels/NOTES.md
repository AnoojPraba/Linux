# CAP Theorem and Consistency Models

## CAP theorem

- **Consistency**: every read sees the most recent write (or an error).
- **Availability**: every request receives a (non-error) response, without
  guaranteeing it's the most recent write.
- **Partition tolerance**: the system keeps operating despite network
  partitions (messages between nodes being dropped/delayed).
- The theorem: you can only guarantee two of the three at once - but in
  practice, partitions WILL happen in any real distributed system, so P
  isn't really a choice. The actual decision is CP vs AP: when a partition
  occurs, do you sacrifice consistency (keep serving, possibly stale/
  conflicting data - AP) or sacrifice availability (refuse/block requests
  on the minority side until the partition heals - CP)?
- Example systems: traditional single-leader RDBMS setups and ZooKeeper/etcd
  lean CP; DynamoDB-style and Cassandra (in default config) lean AP.

## Strong vs eventual consistency

- **Strong consistency**: a read always reflects the latest committed
  write, system-wide. Simpler to reason about for the application, but
  costs latency/availability to coordinate.
- **Eventual consistency**: after writes stop, all replicas will
  *eventually* converge to the same value, but a read immediately after a
  write may return a stale value. Cheaper (no cross-node coordination on
  every read), common in AP systems.

## Quorum reads/writes

- In a system with N replicas, define W = number of replicas that must
  acknowledge a write, and R = number of replicas a read must consult.
- **W + R > N** guarantees a read quorum and a write quorum always overlap
  by at least one node, so a read is guaranteed to see the most recent
  write - giving strong consistency without requiring all N nodes to
  participate in every operation.
- Tuning W and R trades off consistency, latency, and availability: higher
  W/R = stronger consistency guarantees but higher latency and less
  tolerance for node unavailability; lower W/R = faster/more available but
  weaker consistency guarantees.
