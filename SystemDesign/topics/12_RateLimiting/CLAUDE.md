# 12_RateLimiting

Rate-limiting algorithms (token bucket, leaky bucket, fixed and sliding window) and distributed enforcement.

## Files
- `NOTES.md` - (41 lines) sections: Why rate limit; Algorithms; Interview framing

## How to use this note
- Drill: choose an algorithm for a login endpoint vs a public API with bursty clients and say where the counter lives.
- Describe the fixed-window boundary burst (up to 2x) and how sliding window fixes it.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Token bucket allows bursts up to capacity then the refill rate (a good default); leaky bucket smooths output with no burst tolerance.
- Fixed window is simple but allows up to 2x the limit across a boundary; sliding window log is exact but memory heavy, sliding window counter approximates cheaply.
- Distributed limiting needs shared state (e.g. Redis) so limits hold across instances.
- Why: protect backends, enforce tiers, stop abuse (brute force, scraping, DoS).

## Related
- `../03_CachingStrategies`
- `../19_ResiliencePatterns`
- `../20_ServiceDiscoveryAndAPIGateway`
- `../09_MessageQueuesAndEventDrivenArchitecture`
- `../16_SecurityFundamentalsForInterviews`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
