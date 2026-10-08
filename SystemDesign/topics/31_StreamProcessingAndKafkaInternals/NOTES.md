# Stream Processing and Kafka Internals

Queues vs events in general: `09_MessageQueuesAndEventDrivenArchitecture`;
idempotency: `24_...`; transactions/outbox: `29_...`.

## Log-based messaging
- **Kafka = a distributed, replicated, partitioned append-only commit log.**
  Messages are not deleted on consumption; consumers track an **offset** and can
  rewind/replay. Retention by time/size (or **log compaction**: keep the latest
  value per key - a changelog/table).
- Hierarchy: **topic** -> **partitions** (unit of parallelism and ordering) ->
  ordered, immutable records with offsets. Ordering is guaranteed ONLY within a
  partition; choose the partition key (customer id) to keep related events together.
- **Broker storage:** each partition = segment files + sparse offset index +
  time index; writes are sequential appends; reads are sequential (page cache) and
  served with `sendfile` zero-copy (`../../../OS/code/55_ZeroCopyIOAndIoUring`) - why
  Kafka reaches GB/s on commodity disks.
- **Replication:** each partition has a leader and followers; followers fetch from
  the leader. **ISR (in-sync replicas)** = followers caught up; a write is
  committed once all ISR have it (`acks=all` + `min.insync.replicas`); leader
  election picks from the ISR (KRaft/ZooKeeper coordinates metadata). Unclean
  leader election trades durability for availability.
- **Producer:** batching (`linger.ms`, `batch.size`), compression, partitioner,
  `acks` (0/1/all), retries, **idempotent producer** (producer id + sequence
  numbers dedupe retries per partition), **transactions** (atomic writes across
  partitions + consumer offsets = exactly-once for read-process-write within Kafka).
- **Consumer groups:** partitions are divided among consumers in a group (max
  parallelism = number of partitions); **rebalances** (consumer join/leave/crash)
  pause consumption (cooperative/incremental rebalancing and static membership
  reduce this). Different groups read independently (pub/sub); within a group it's
  a queue. Offsets committed to `__consumer_offsets`; commit AFTER processing for
  at-least-once, before for at-most-once.
- **Delivery semantics:** at-most-once (commit then process), at-least-once
  (process then commit; duplicates on crash), exactly-once = idempotent producer +
  transactions + `read_committed` consumers (or idempotent sinks).
- **Back-pressure/lag:** consumers pull; **consumer lag** = log end offset minus
  committed offset - the key health metric. Slow consumer -> lag grows until
  retention deletes unread data.
- Sizing: partitions = max(target throughput / per-partition throughput,
  consumer parallelism); too many partitions increase metadata, rebalance and
  failover time. Hot partitions from skewed keys are a classic problem.

## Compared to other brokers
| | Kafka/Pulsar/Kinesis (log) | RabbitMQ/SQS (queue) |
|---|---|---|
| Retention | replayable log | deleted on ack |
| Ordering | per partition | per queue (limited with competing consumers) |
| Throughput | very high, sequential | moderate |
| Routing | topic/partition key | rich exchanges/routing (RabbitMQ) |
| Use | event streaming, CDC, analytics, event sourcing | task queues, RPC-ish work distribution |

## Stream processing (Flink, Kafka Streams, Spark Structured Streaming, Beam)
- Continuous computation over unbounded data: map/filter, keyed aggregations,
  joins, enrichment.
- **Event time vs processing time:** events arrive late/out of order; use
  **watermarks** ("no events older than T expected") to decide when a window is
  complete; **allowed lateness** + side outputs for stragglers.
- **Windows:** tumbling (fixed, non-overlapping), sliding (overlapping), session
  (gap-based), global.
- **State:** keyed state in local stores (RocksDB - an LSM, `30_...`), made
  fault-tolerant by periodic **checkpoints/snapshots** (Chandy-Lamport barriers in
  Flink) + replayable sources -> exactly-once state updates; changelog topics in
  Kafka Streams.
- **Delivery to sinks:** idempotent upserts or two-phase-commit sinks for
  end-to-end exactly-once.
- **Lambda vs Kappa architecture:** Lambda = batch + speed layers (two code
  paths); Kappa = everything is a stream, reprocess by replaying the log.
- **CDC (change data capture):** Debezium turns DB WAL into a Kafka stream -
  keeps search indexes, caches, and warehouses in sync (see `29_...` outbox).

## Common problems
Poison messages (dead-letter topics), schema evolution (schema registry, Avro/
Protobuf with compatibility rules), duplicate handling, skewed partitions,
unbounded state growth, rebalance storms, clock skew, reprocessing costs,
ordering across partitions (not guaranteed), large messages (store payload
elsewhere).

## Senior interviewer Q&A
**Q: Why is Kafka so fast?**
A: Sequential disk appends and reads, OS page cache, batching and compression,
zero-copy `sendfile` to sockets, partitioned parallelism, and a dumb broker
(consumers pull and track their own offsets).

**Q: How do you guarantee ordering in Kafka?**
A: Only within a partition: use a key so related events share a partition, one
consumer per partition per group, `max.in.flight.requests` <= 5 with idempotent
producer (preserves order on retries). Global ordering requires one partition
(no parallelism).

**Q: Explain exactly-once in Kafka.**
A: Idempotent producer dedupes retries; transactions atomically write output
records and commit consumed offsets; consumers use `isolation.level=read_committed`.
Guarantee holds inside Kafka; side effects in external systems still need
idempotency or transactional sinks.

**Q: What happens when a consumer in the group dies?**
A: The group coordinator detects missed heartbeats/session timeout, triggers a
rebalance, and its partitions are reassigned; the new owner resumes from the last
committed offset (so uncommitted work is reprocessed - hence idempotent processing).

**Q: How would you handle consumer lag spikes?**
A: Scale consumers up to partition count, increase partitions (beware of key
remapping), optimize processing/batching, fix hot keys, add backpressure to
producers, check rebalance storms/GC pauses, and alert on lag growth rate not
absolute value.

**Q: Event time windows with late data - how?**
A: Watermarks track event-time progress; windows fire when the watermark passes
their end; allowed lateness keeps state for late updates; later data goes to a
side output or triggers retractions/updates.

**Q: When is Kafka the wrong tool?**
A: Simple low-volume task queues with per-message routing/priorities/delays
(RabbitMQ/SQS), request/response RPC, tiny deployments where operational cost
dominates, or strict global ordering needs.
