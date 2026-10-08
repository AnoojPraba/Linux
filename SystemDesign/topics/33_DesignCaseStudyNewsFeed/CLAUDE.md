# 33_DesignCaseStudyNewsFeed

Case study: news feed / timeline (requirements, estimates, architecture, fan-out on write vs read vs hybrid, data model, read path and hard problems), with a senior Q&A.

## Files
- `NOTES.md` - (114 lines) sections: 1. Requirements (clarify first); 2. Estimates; 3. High-level architecture; 4. Fan-out: the central trade-off; 5. Data model & storage; 6. Read path details; 7. Hard problems; Senior interviewer Q&A

## How to use this note
- Whiteboard in order: requirements, estimates (QPS, fan-out inserts/s, cache size), architecture, fan-out trade-off, data model, read path, hard problems.
- Rehearse the "Senior interviewer Q&A": push vs pull, pagination, celebrity post, like counts, cache loss, adding ranking.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Fan-out trade-off: push (fast reads, write amplification, wasted on inactive users), pull (cheap writes, slow reads), hybrid with a follower-count threshold (push for normal users, pull for celebrities, skip inactive users).
- Feed cache entries are (user_id, post_id, ts) in capped Redis lists/zsets; the cache is derived data and can be rebuilt from posts plus follows.
- Read path: id list from cache, merge celebrity posts, hydrate with batched multi-get, optional two-stage ranking, filter, return page plus cursor.
- Keyset/cursor pagination on (timestamp, id), not offsets.
- Hard problems: hot keys (replicate, coalesce), thundering herd (single-flight, jitter), lazy unfollow/delete filtering, cold-start rebuild, multi-region, counters (sharded, approximate).

## Related
- `../14_DesignCaseStudyChatSystem`
- `../03_CachingStrategies`
- `../31_StreamProcessingAndKafkaInternals`
- `../17_CapacityEstimationAndBackOfEnvelopeMath`
- `../32_CausalityVectorClocksAndCRDTs`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
