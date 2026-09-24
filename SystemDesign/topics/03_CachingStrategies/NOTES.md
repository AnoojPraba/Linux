# Caching Strategies

## Where caches live

- **CDN (Content Delivery Network)**: edge caching, physically close to
  users, for static/semi-static content (images, JS/CSS bundles, video) or
  cacheable API responses. Cuts latency (shorter round trip) and origin load.
- **In-memory caches (Redis/Memcached)**: sit between the application and
  the database, typically holding hot, frequently-read data as key-value
  pairs. Redis adds richer data structures and optional persistence;
  Memcached is a simpler, pure in-memory cache.

## Cache invalidation strategies

- **TTL (time-to-live)**: entries expire automatically after a fixed time.
  Simple, bounds staleness, but staleness *does* occur within the TTL
  window.
- **Cache-aside (lazy loading)**: application checks the cache first; on a
  miss, reads from the DB and populates the cache. Cache only holds what's
  actually been requested, but the first request after eviction/expiry pays
  full DB latency.
- **Write-through**: writes go to the cache and the DB together
  (synchronously) - cache is always consistent with the DB, at the cost of
  slower writes.
- **Write-back / write-behind**: writes go to the cache immediately and are
  flushed to the DB asynchronously later - fast writes, but risks data loss
  if the cache node fails before the flush.
- The joke: "there are only two hard problems in computer science: cache
  invalidation, naming things, and off-by-one errors" - invalidation is hard
  because deciding *when* cached data is stale enough to refresh, without
  either serving stale data or refreshing so often the cache is useless, is
  a genuinely nontrivial tradeoff.

## Cache stampede / thundering herd

- Problem: a hot key expires (or the cache/service restarts cold), and a
  huge burst of concurrent requests all miss the cache simultaneously and
  hammer the database at once, potentially taking it down.
- Mitigations:
  - **Jittered TTLs**: randomize expiry times slightly so many keys don't
    expire in the same instant.
  - **Request coalescing / locking**: on a miss, only one request actually
    goes to the DB to repopulate the cache; concurrent requests for the same
    key wait for that result instead of each hitting the DB independently.
  - Serve slightly-stale data while a background refresh happens
    ("stale-while-revalidate") instead of blocking every caller on a fresh
    fetch.
