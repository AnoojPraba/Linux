# 03_CachingStrategies

Where caches live (CDN, in-memory), invalidation strategies (TTL, cache-aside, write-through, write-back) and stampede mitigation.

## Files
- `NOTES.md` - (47 lines) sections: Where caches live; Cache invalidation strategies; Cache stampede / thundering herd

## How to use this note
- Drill: pick a read-heavy feature, choose a cache placement and write policy, then say what happens when a hot key expires.
- Quantify hit ratio and staleness tolerance before choosing TTL.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- CDN for static/semi-static content near users; Redis/Memcached between app and DB.
- Cache-aside caches only what is requested but the first miss pays full latency; write-through is consistent but slower; write-back is fast but loses data if the cache dies.
- TTL bounds staleness but allows staleness within the window; invalidation is the hard part.
- Stampede: jittered TTLs, request coalescing/locking, stale-while-revalidate.

## Related
- `../15_DesignCaseStudyDistributedCache`
- `../01_ScalabilityBasics`
- `../12_RateLimiting`
- `../../../C_Basics/code/37_LRUCache`
- `../../../C_Basics/code/35_ProbabilisticDataStructures`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
