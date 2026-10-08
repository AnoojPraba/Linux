# Design Case Study: News Feed / Timeline

Related: `03_CachingStrategies`, `07_DatabaseShardingAndReplication`,
`09_MessageQueuesAndEventDrivenArchitecture`, `17_CapacityEstimationAndBackOfEnvelopeMath`,
`14_DesignCaseStudyChatSystem`, `31_StreamProcessingAndKafkaInternals`.

## 1. Requirements (clarify first)
- Functional: post (text/media), follow/unfollow, view home feed (posts from people I
  follow, ranked or reverse-chronological), pagination/infinite scroll, likes/comments
  (counts), notifications optional.
- Non-functional: feed read latency p99 < 200-500 ms; high availability; eventual
  consistency acceptable (a post may take seconds to appear); scale: 300M DAU,
  read:write ~ 100:1+, celebrities with 100M followers.
- Out of scope unless asked: ads, ML ranking details, search.

## 2. Estimates
- 300M DAU x 10 feed loads/day = 3B reads/day ~ 35k QPS avg, 100k+ peak.
- 100M posts/day ~ 1.2k writes/s avg. Fan-out: avg 200 followers -> 20B
  feed-entries/day if pushed ~ 230k inserts/s - big but just ids.
- Storage: post ~ 1 KB metadata -> 100 GB/day; media in object store + CDN (PBs).
- Feed entry = (user_id, post_id, ts) ~ 24 B; keep the latest ~800 per user in
  cache: 300M x 800 x 24 B ~ 5.8 TB (sharded Redis cluster).

## 3. High-level architecture
```
Client -> API gateway/LB -> Post service  -> posts DB (sharded by post_id/user) + object store (media -> CDN)
                         -> Graph service -> follower/following (sharded by user_id)
                         -> Feed service  -> feed cache (Redis lists/zsets per user)
Post service -> Kafka "post-created" -> Fan-out workers -> feed cache (push)
                                     -> Notification, search indexer, analytics
Feed read: feed cache (ids) -> hydrate posts/users (multi-get cache) -> ranker -> response
```

## 4. Fan-out: the central trade-off
| | Fan-out on WRITE (push) | Fan-out on READ (pull) | Hybrid |
|---|---|---|---|
| On post | push post id into every follower's feed cache | store once | push for normal users only |
| On feed read | read precomputed list (fast) | query posts of all followees, merge, sort | read pushed list + pull celebrity posts, merge |
| Cost | write amplification (celebrity = 100M inserts); wasted work for inactive users | read amplification, slow reads | best of both |
| Freshness | slight delay for fan-out | immediate | mixed |
- **Hybrid** (Twitter/Instagram approach): push for typical users, pull for
  celebrities (threshold on follower count), and skip fan-out for inactive users
  (they build their feed on next login).
- Fan-out workers consume from Kafka (partitioned by author) in parallel;
  batch writes; idempotent inserts (`post_id` dedupe); retry/DLQ.

## 5. Data model & storage
- `posts(post_id, author_id, text, media_ids, created_at)` - sharded; ids from
  Snowflake-style generator (time-sortable, `32_...`).
- `follows(follower_id, followee_id)` stored both ways (who I follow / my
  followers) in a sharded KV/wide-column store or graph store.
- Feed cache: Redis sorted set / list per user (score = timestamp or rank),
  capped length (e.g. 800), TTL for inactive users; persisted source of truth =
  recompute from posts + follows if lost.
- Likes/comments: counters (approximate, sharded/CRDT-friendly `32_...`), async
  aggregation; hot posts need counter sharding.
- Media: upload to object storage via pre-signed URLs, async transcode, CDN.

## 6. Read path details
1. Get my feed id list from cache (cursor-based pagination: `(last_ts, post_id)`,
   NOT offset - stable under inserts).
2. Merge in celebrity posts (pull) for followees above the threshold.
3. **Hydrate:** batch `MGET` post bodies, author profiles, like counts, "liked by
   me" (cache with high hit rate; request coalescing for hot keys).
4. **Rank** (optional): lightweight scoring (recency, affinity, engagement) or an
   ML model on the top ~500 candidates; cache results briefly.
5. Filter (blocked/muted, privacy), dedupe, return page + next cursor.

## 7. Hard problems
- **Celebrity/hot keys:** pull model, replicate hot keys across cache nodes
  (local in-process cache with short TTL), request coalescing.
- **Thundering herd** on cache miss/regeneration: single-flight, jittered TTLs.
- **Unfollow/delete:** lazily filter at read time (tombstones) + async cleanup
  instead of rewriting millions of feeds.
- **Ordering and consistency:** eventual; "read your writes" for the author (inject
  own post into own feed on the client/session); monotonic reads via sticky cache.
- **Cold start / cache loss:** rebuild from posts + follows (pull for the first
  request, then warm), rate-limit rebuilds.
- **Backpressure/failures:** Kafka lag alerts, degrade gracefully (serve stale feed,
  skip ranking), circuit breakers (`19_ResiliencePatterns`).
- **Multi-region:** replicate feed/posts per region, route users to nearest, async
  cross-region replication; conflict-free since feeds are append/derived.
- **Observability:** fan-out lag, feed latency percentiles, cache hit rate, per-shard
  skew (`21_ObservabilityLogsMetricsTraces`). **Abuse:** rate limiting (`12_...`).

## Senior interviewer Q&A
**Q: Push vs pull - which and why?**
A: Neither alone. Push gives fast reads (the dominant traffic) at write
amplification cost; pull avoids amplification for celebrities. Hybrid with a
follower-count threshold, skipping inactive users, balances cost and latency.

**Q: How do you paginate the feed?**
A: Cursor/keyset pagination using `(timestamp, id)`; offsets shift as new posts
arrive causing duplicates/skips and are O(n) in DBs.

**Q: A celebrity with 100M followers posts. What happens?**
A: With hybrid, no fan-out; followers' feeds pull that author's recent posts at
read time (cached heavily, hot key replicated). If pushing were used it would
take minutes and spike the cache/queue cluster.

**Q: How would you keep like counts accurate and cheap?**
A: Increment async via a queue/sharded counters (many sub-counters summed),
cache the aggregate, accept slight staleness; periodically reconcile from the
source of truth; dedupe per user with a set/bitmap.

**Q: What breaks if the feed cache is lost?**
A: The cache is derived data: rebuild per user on demand from follows + recent
posts (pull), throttle rebuild concurrency, keep durable snapshots/replicas, and
prefer replicating cache shards.

**Q: How do you add ranking without hurting latency?**
A: Two-stage: cheap candidate generation from the cached list, then a bounded
ranker over ~hundreds of items with precomputed features; cache results; fall back
to chronological on timeout.
