# 02_LoadBalancing

L4 vs L7 load balancing, balancing algorithms (round robin, least connections, consistent hashing) and liveness vs readiness health checks.

## Files
- `NOTES.md` - (46 lines) sections: L4 vs L7 load balancing; Load balancing algorithms; Health checks

## How to use this note
- Drill: draw edge L4 -> L7 -> services, then explain what consistent hashing buys over `hash(key) % N`.
- Be ready to justify an algorithm per workload (uniform vs variable request cost, cache affinity).
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- L4: fast, cheap, blind to HTTP; L7: path/header/cookie routing and TLS termination, more CPU; layered in practice.
- Round robin for uniform requests, least connections when cost varies, consistent hashing for cache/shard affinity.
- Consistent hashing remaps only about 1/N of keys when a node joins or leaves.
- Health checks: liveness (process up) vs readiness (can serve); readiness-only failures cause errors if you check liveness alone.

## Gotchas
- An untracked vim swap file (`.NOTES.md.swp`) sits next to NOTES.md: the note was open in an editor; it is not content and should not be committed.

## Related
- `../01_ScalabilityBasics`
- `../03_CachingStrategies`
- `../07_DatabaseShardingAndReplication`
- `../20_ServiceDiscoveryAndAPIGateway`
- `../../../OS/code/54_IOMultiplexing`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
