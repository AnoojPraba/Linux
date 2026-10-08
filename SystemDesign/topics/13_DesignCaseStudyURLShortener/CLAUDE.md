# 13_DesignCaseStudyURLShortener

Case study: URL shortener (requirements questions, ID generation approaches, schema and caching, what interviewers listen for).

## Files
- `NOTES.md` - (50 lines) sections: Requirements clarification (ask before designing); ID generation approaches; Database schema and caching; What interviewers are actually listening for

## How to use this note
- Whiteboard it in 45 minutes: clarify read/write ratio and expiry, estimate writes/s and reads/s, pick an ID scheme, then add cache and discuss 301 vs 302.
- The schema is the least interesting part; spend time on trade-offs and estimation.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Read-heavy: far more redirects than creations, so cache `short_code -> long_url` (cache-aside) in front of the DB; links follow a power-law hot-key distribution.
- Counter + base62: no collisions, short codes, but needs a coordinated counter or reserved ranges per server.
- Hash prefix: no coordination but collisions must be detected and retried (salt).
- Pre-generated key pool: no hot-path coordination or collision handling, but you manage the pool.
- 301 (cacheable, fewer hits, harder analytics) vs 302 (every click hits you, easy tracking); custom aliases and expiry add requirements.

## Related
- `../03_CachingStrategies`
- `../17_CapacityEstimationAndBackOfEnvelopeMath`
- `../07_DatabaseShardingAndReplication`
- `../12_RateLimiting`
- `../28_CommonInterviewQuestionsCheatSheet`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
