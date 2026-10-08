# 01_ScalabilityBasics

Foundation note: vertical vs horizontal scaling, stateless vs stateful services and the bottleneck-first scaling mindset.

## Files
- `NOTES.md` - (45 lines) sections: Vertical vs horizontal scaling; Stateless vs stateful services; The general scaling mindset

## How to use this note
- Warm-up drill: explain out loud why statelessness enables horizontal scaling, then name where the bottleneck moves next (usually the database).
- Always start a design by identifying the real bottleneck resource before adding servers.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Vertical scaling: simple, hard ceiling, single point of failure; horizontal: unbounded and fault tolerant but needs stateless design, load balancing and coordination.
- Stateless services let any instance serve any request; push state to Redis/DB (or use sticky sessions at a cost).
- Scaling a tier just moves the bottleneck downstream, which motivates caching, sharding and replication.
- Real systems combine both: scale nodes to a sane price point, then scale out.

## Gotchas
- An untracked vim swap file (`.NOTES.md.swp`) sits next to NOTES.md: the note was open in an editor; it is not content and should not be committed.

## Related
- `../02_LoadBalancing`
- `../03_CachingStrategies`
- `../07_DatabaseShardingAndReplication`
- `../17_CapacityEstimationAndBackOfEnvelopeMath`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
