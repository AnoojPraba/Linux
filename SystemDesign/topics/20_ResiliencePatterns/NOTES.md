# Resilience Patterns

## Circuit breaker

- After N consecutive failures calling a downstream service, "trip" the
  breaker and fail fast (or fall back) instead of continuing to hammer a
  struggling service.
- States: **closed** (normal, requests flow through) -> **open** (failing
  fast for a cooldown period) -> **half-open** (let a probe request
  through to test recovery) -> back to closed if it succeeds, or open
  again if it still fails.

## Retry with exponential backoff and jitter

- Naive immediate retries make an overloaded downstream service worse -
  a flood of instant retries is a "retry storm" that can turn a brief
  blip into a full outage.
- Exponential backoff: space retries out (e.g. 1s, 2s, 4s, 8s...) so the
  downstream service gets breathing room to recover.
- Jitter: randomize the backoff so many clients retrying after the same
  failure don't all retry in lockstep and re-synchronize the storm.

## Bulkhead isolation

- Partition resources (thread pools, connection pools) per downstream
  dependency, so one slow/failing dependency can't exhaust resources
  needed for calls to other, healthy dependencies.
- Named after ship bulkheads that contain flooding to a single
  compartment instead of sinking the whole ship.

## Timeouts

- Every network call needs an explicit timeout. Without one, a hung
  downstream call can tie up caller threads/connections indefinitely and
  cascade the outage upstream.

## Interview framing

- These patterns are what actually makes messaging/event-driven systems
  (`09_MessageQueuesAndEventDrivenArchitecture`) and service-to-service
  calls behind an `21_ServiceDiscoveryAndAPIGateway` gateway survive
  partial failures gracefully instead of cascading.
