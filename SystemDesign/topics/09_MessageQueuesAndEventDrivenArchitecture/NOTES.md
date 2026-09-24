# Message Queues and Event-Driven Architecture

## Why async decoupling

- A producer publishes a message and moves on without waiting for the
  consumer to process it - decouples the producer's latency/availability
  from the consumer's, and lets consumers process at their own pace
  (buffering bursts instead of the producer blocking or failing under
  load).
- Enables independent scaling of producers and consumers, and lets you add
  new consumers of the same events later without touching the producer.

## Delivery semantics

- **At-most-once**: message is sent once, no retry on failure - safe for
  non-idempotent work, but a lost message is just lost.
- **At-least-once**: message is retried until acknowledged, so it may be
  delivered more than once - requires idempotent consumers (or
  dedup logic) to be safe.
- **Exactly-once**: delivered and processed exactly one time - the ideal,
  but genuinely hard to guarantee end-to-end in a distributed system;
  usually approximated via at-least-once delivery plus idempotency keys on
  the consumer side rather than achieved as a true primitive.
- This is the same delivery-semantics tradeoff covered from the RPC angle
  in `../../OS/code/51_RpcMechanisms/NOTES.md` and
  `../../Cpp/code/31_RpcMechanismsCpp` - same at-most-once/at-least-once/
  exactly-once vocabulary, just applied to async messaging instead of a
  synchronous remote call.

## Pub/sub vs point-to-point

- **Point-to-point (queue)**: one message is consumed by exactly one
  consumer among a pool of workers - classic work-queue/task-distribution
  pattern.
- **Pub/sub (topic)**: a message is delivered to every subscriber of the
  topic - used when multiple independent services all need to react to the
  same event (e.g. an "order placed" event fanning out to billing,
  shipping, and analytics).

## Backpressure and dead-letter queues

- **Backpressure**: when consumers can't keep up with producers, the queue
  fills up - systems need a strategy (block producers, drop messages, scale
  out consumers, apply rate limiting upstream) rather than let the queue
  grow unbounded.
- **Dead-letter queue (DLQ)**: messages that repeatedly fail processing are
  routed to a separate queue instead of retried forever, so poison messages
  don't block the main queue and can be inspected/reprocessed manually.

## Concrete examples

- **Kafka**: log-based - messages are appended to a durable, ordered,
  replayable log partitioned by key; consumers track their own offset, so
  multiple consumer groups can independently re-read history. Ordering is
  guaranteed within a partition, not across the whole topic. Great for
  high-throughput event streaming and replay.
- **RabbitMQ**: a traditional message broker with flexible routing
  (exchanges/bindings/routing keys) and per-message acknowledgment -
  strong at complex routing topologies and point-to-point work queues, but
  messages are generally consumed and gone (not natively replayable like a
  Kafka log).
