# Rate Limiting

## Why rate limit

- Protect backend services from being overloaded by traffic spikes (or
  abusive clients).
- Enforce fair usage / API pricing tiers (e.g. free tier gets 100 req/min,
  paid tier gets 10,000 req/min).
- Prevent abuse: brute-force login attempts, scraping, denial-of-service.

## Algorithms

- **Token bucket**: a bucket holds up to N tokens, refilled at a fixed
  rate; each request consumes a token, and requests are rejected/queued
  when the bucket is empty. Allows short bursts up to the bucket's
  capacity, then settles to the steady refill rate - a good default for
  most APIs.
- **Leaky bucket**: requests enter a queue and are processed (leak out) at
  a fixed, constant rate regardless of burstiness of arrival - smooths
  output perfectly but doesn't allow any burst tolerance, and excess
  requests either queue or are dropped.
- **Fixed window counter**: count requests in fixed time windows (e.g.
  per-minute buckets), reset the counter each window. Simple to implement,
  but allows up to 2x the intended rate right at a window boundary (a
  client can send the full quota at the very end of one window and the
  full quota again at the very start of the next).
- **Sliding window (log or counter)**: tracks request timestamps (log) or a
  weighted blend of the current and previous fixed windows (counter) to
  avoid the boundary-burst problem. Sliding window log is exact but costs
  more memory (storing every timestamp); sliding window counter
  approximates it far more cheaply.

## Interview framing

- Pick an algorithm based on whether bursts should be tolerated (token
  bucket) or strictly smoothed (leaky bucket), and whether exact-rate
  correctness at window boundaries matters enough to justify sliding
  window's extra cost over a fixed window counter.
- In a distributed system, rate-limit state (the counter/bucket) usually
  needs to live in a shared store (e.g. Redis) so limits are enforced
  consistently across multiple API server instances, not per-instance.
