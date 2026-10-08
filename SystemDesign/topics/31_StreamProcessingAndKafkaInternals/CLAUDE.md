# 31_StreamProcessingAndKafkaInternals

Kafka as a partitioned replicated commit log (storage, replication/ISR, producers, consumer groups, delivery semantics) and stream-processing concepts (event time, watermarks, windows, state, Lambda vs Kappa), with a senior Q&A.

## Files
- `NOTES.md` - (115 lines) sections: Log-based messaging; Compared to other brokers; Stream processing (Flink, Kafka Streams, Spark Structured Streaming, Beam); Common problems; Senior interviewer Q&A

## How to use this note
- Drill: design a topic (partition key, partition count, retention) and say how consumers scale, what a rebalance does and how lag is monitored.
- Rehearse the "Senior interviewer Q&A": why Kafka is fast, ordering, exactly-once, consumer failure, lag spikes, late data, when Kafka is the wrong tool.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Ordering is guaranteed only within a partition; the partition key keeps related events together; parallelism is capped at partition count.
- Writes are sequential appends served through page cache and sendfile zero-copy; commit requires the ISR (`acks=all` + `min.insync.replicas`).
- Delivery: commit after processing = at-least-once, before = at-most-once; exactly-once inside Kafka = idempotent producer + transactions + `read_committed`; external sinks still need idempotency.
- Consumer groups rebalance on join/leave/crash and pause consumption; consumer lag is the key health metric.
- Stream processing: event time vs processing time, watermarks and allowed lateness, tumbling/sliding/session windows, checkpointed keyed state (RocksDB), CDC with Debezium.
- Problems: poison messages, schema evolution, hot partitions, rebalance storms, unbounded state.

## Related
- `../09_MessageQueuesAndEventDrivenArchitecture`
- `../29_DistributedTransactions2PCSagaOutbox`
- `../30_LSMTreesAndStorageEngines`
- `../24_IdempotencyInDistributedSystems`
- `../../../OS/code/55_ZeroCopyIOAndIoUring`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
