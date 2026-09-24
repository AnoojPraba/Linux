# Database Sharding and Replication

## Sharding strategies

- **Range-based**: partition by key ranges (e.g. user IDs 1-1M on shard 1,
  1M-2M on shard 2). Easy to reason about and supports efficient range
  scans, but can create uneven load if certain ranges are accessed far more
  than others (e.g. recently-created users all land on the newest shard).
- **Hash-based**: hash the shard key to pick a shard - spreads load evenly
  on average, at the cost of losing range-scan locality (a range query now
  has to fan out to every shard). Consistent hashing (see
  `../02_LoadBalancing`) is the standard way to do this while minimizing
  reshuffling when shards are added/removed.
- **Geographic**: partition by region (e.g. EU users on EU shards) -
  reduces latency for regional users and can help with data-residency/
  compliance requirements, but complicates cross-region queries and
  requires care for users who move/travel.
- **Hot shard problem**: a single shard receives disproportionate traffic
  (a celebrity user, a viral piece of content) and becomes a bottleneck
  even though the cluster overall has capacity. Mitigations: choosing a
  higher-cardinality/more evenly-distributed shard key, splitting further,
  or caching in front of the hot shard.

## Replication

- **Leader-follower (primary-replica)**: writes go to the leader, which
  replicates to followers; followers can serve reads, which scales read
  throughput. Followers also serve as failover targets if the leader dies.
- **Replication lag**: followers apply writes slightly after the leader, so
  a read from a follower can return stale data ("read-your-own-writes"
  problems if a client writes to the leader then immediately reads from a
  lagging follower). Mitigations: route a given user's read to the leader
  right after their own write, or use session-consistency tricks.
- **Synchronous vs asynchronous replication**:
  - *Synchronous*: leader waits for follower(s) to acknowledge before
    confirming the write - stronger durability (a follower already has the
    data if the leader dies right after), but higher write latency and the
    leader can stall if a follower is slow/down.
  - *Asynchronous*: leader confirms the write immediately without waiting -
    lower latency, but a leader crash right after can lose the most recent
    writes that never reached a follower.
