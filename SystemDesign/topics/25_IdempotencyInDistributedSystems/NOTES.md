# Idempotency in Distributed Systems

## Why idempotency matters

- In a distributed system, a client (or an at-least-once delivery queue)
  may retry a request that actually succeeded server-side but whose
  response was lost (e.g. a network blip).
- Without idempotency, a naive retry causes a duplicate side effect (e.g.
  double-charging a payment, double-sending an email).

## Idempotency key pattern

- Client generates a unique key per logical operation (e.g. a UUID) and
  sends it with the request.
- Server checks whether it has already processed that key; if so, it
  returns the cached original result instead of re-executing the side
  effect.

## HTTP methods and idempotency

- Naturally idempotent: GET, PUT, DELETE - repeating them should produce
  the same end state.
- Not naturally idempotent: POST - repeating it typically creates a new
  resource each time, which is exactly why POST-based APIs (e.g. payment
  APIs) need the idempotency-key pattern.

## Interview framing

- `09_MessageQueuesAndEventDrivenArchitecture` covers at-least-once
  delivery semantics - idempotency is exactly the technique that makes
  at-least-once delivery safe to use in practice: "at least once
  delivered" + "idempotent handler" = an effective "exactly once" outcome
  from the caller's perspective, without needing true exactly-once
  delivery (which is expensive/hard to guarantee end-to-end).
