# 20_ServiceDiscoveryAndAPIGateway

Service discovery (client-side vs server-side) and the API gateway pattern with its cross-cutting concerns and trade-offs.

## Files
- `NOTES.md` - (36 lines) sections: Service discovery; API Gateway; Interview framing

## How to use this note
- Drill: explain how a new instance becomes reachable internally (discovery) and externally (gateway).
- Name what belongs in the gateway and what must stay out (business logic).
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Client-side discovery queries a registry (Consul, etcd, ZooKeeper) and picks an instance; server-side discovery hides it behind a load balancer/router.
- Gateway centralises auth, rate limiting, routing/composition, TLS termination and response caching.
- Risks: gateway as single point of failure/bottleneck (scale and replicate it) and as a "god object" if business logic creeps in.
- Pair with timeouts and circuit breakers at the call boundary.

## Related
- `../02_LoadBalancing`
- `../12_RateLimiting`
- `../19_ResiliencePatterns`
- `../22_ConsensusAndCoordination`
- `../35_ContainersAndKubernetesBasics`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
