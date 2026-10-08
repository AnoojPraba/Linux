# 09_MessageQueuesAndEventDrivenArchitecture

Async decoupling with message queues: delivery semantics, pub/sub vs point-to-point, backpressure, dead-letter queues, with Kafka and RabbitMQ contrasted.

## Files
- `NOTES.md` - (61 lines) sections: Why async decoupling; Delivery semantics; Pub/sub vs point-to-point; Backpressure and dead-letter queues; Concrete examples

## How to use this note
- Drill: design an "order placed" fan-out; state delivery semantics, ordering key, retry/DLQ policy and what happens when consumers fall behind.
- Say explicitly how consumers stay idempotent under at-least-once delivery.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- At-most-once loses messages, at-least-once duplicates them (needs idempotent consumers), exactly-once is approximated by at-least-once plus idempotency keys.
- Queue = one consumer per message (work distribution); topic = every subscriber gets it (fan-out).
- Backpressure options: block producers, drop, scale consumers, rate limit upstream; never let a queue grow unbounded.
- DLQ isolates poison messages for inspection and replay.
- Kafka: partitioned replayable log, consumer-tracked offsets, ordering per partition only; RabbitMQ: flexible routing and per-message acks, messages consumed and gone.

## Related
- `../24_IdempotencyInDistributedSystems`
- `../31_StreamProcessingAndKafkaInternals`
- `../29_DistributedTransactions2PCSagaOutbox`
- `../19_ResiliencePatterns`
- `../../../OS/code/56_RpcMechanisms`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
