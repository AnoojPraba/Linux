# 10_APIProtocolsRESTvsGRPCvsGraphQL

REST vs gRPC vs GraphQL: strengths, weaknesses and where each fits.

## Files
- `NOTES.md` - (45 lines) sections: REST; gRPC; GraphQL

## How to use this note
- Drill: pick a protocol for a public mobile API, an internal service mesh and a dashboard aggregating many sources, and justify each.
- Know the caching story for each (REST best, GraphQL hardest).
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- REST: resources plus HTTP verbs, stateless, naturally cacheable; over-fetching and under-fetching.
- gRPC: protobuf binary, HTTP/2 streaming, generated typed clients; ideal internal; needs gRPC-Web/proxy for browsers and is harder to debug.
- GraphQL: client chooses fields in one round trip, single endpoint; HTTP caching is hard and naive resolvers recreate N+1 (use DataLoader batching).
- API evolution: additive changes, versioning, deprecation.

## Related
- `../04_DatabaseIndexingAndQueryOptimization`
- `../20_ServiceDiscoveryAndAPIGateway`
- `../24_IdempotencyInDistributedSystems`
- `../../../OS/code/56_RpcMechanisms`
- `../../../Cpp/code/32_RpcMechanismsCpp`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
