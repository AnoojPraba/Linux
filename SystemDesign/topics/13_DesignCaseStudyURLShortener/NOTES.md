# Design Case Study: URL Shortener

The "hello world" of system design interviews - simple enough to fit in 45
minutes, but rich enough to probe requirements-gathering, capacity
estimation, and tradeoff articulation.

## Requirements clarification (ask before designing)

- Read-heavy or write-heavy? (Typically read-heavy: far more redirects than
  new shortlinks created.)
- Do users need custom aliases (`bit.ly/my-brand`) or only generated codes?
- Do links expire? Is click analytics required (count, referrer, geo)?
- Redirect type: 301 (permanent, browser/CDN caches it, fewer hits to your
  service but harder to update analytics) vs 302 (temporary, every click
  hits your service, easier to track).

## ID generation approaches

- **Counter + base62 encoding**: a monotonically increasing counter (needs
  to be coordinated across servers - e.g. a dedicated ID-generation service
  or reserved ID ranges per server) encoded into a short base62 string.
  Guarantees no collisions, short codes, but needs a coordination point for
  the counter.
- **Hash-based**: hash the long URL (e.g. MD5/SHA) and take a prefix of the
  result as the code. No coordination needed, but collisions are possible
  and must be detected/handled (retry with a salt, append a disambiguating
  character).
- **Pre-generated key pool**: a background job generates and stores random
  unused short codes ahead of time; the write path just claims one from the
  pool. Avoids both live coordination and collision-handling on the hot
  path, at the cost of managing the pool.

## Database schema and caching

- Minimal schema: `short_code (PK)`, `long_url`, `created_at`,
  `expires_at`, optionally `owner_id`/`click_count`.
- Read path (redirect) is the hot path: cache `short_code -> long_url`
  mappings in an in-memory cache (see `../03_CachingStrategies`, cache-aside
  is a natural fit) in front of the database, since a small fraction of
  links usually receive most of the traffic (power-law/hot-key
  distribution).

## What interviewers are actually listening for

- Asking clarifying questions before jumping to a schema.
- Basic capacity estimation (writes/sec, reads/sec, storage growth over
  time) to justify design choices out loud.
- Articulating tradeoffs explicitly (hash vs counter, 301 vs 302, cache-
  aside vs write-through) rather than presenting one design as the only
  option - the schema itself is the least interesting part of the answer.
