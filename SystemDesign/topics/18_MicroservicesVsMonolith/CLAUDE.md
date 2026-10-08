# 18_MicroservicesVsMonolith

Monolith vs microservices trade-offs, the distributed-monolith anti-pattern and Conway's Law.

## Files
- `NOTES.md` - (38 lines) sections: Monolith; Microservices; The "distributed monolith" anti-pattern; General guidance

## How to use this note
- Drill: argue both sides for a mid-size team, then state the trigger that would make you split a service.
- Default answer: start monolithic, split for a clear scaling or ownership reason.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Monolith: simple dev/test/deploy and easy ACID across one DB; scaling is all-or-nothing and team coordination degrades as it grows.
- Microservices: independent deploy/scale and team autonomy; cost is network latency, no cross-service ACID (sagas, eventual consistency) and operational overhead.
- Distributed monolith: shared DB, synchronous chains and lockstep deploys give all the cost and none of the benefit.
- Conway's Law: architecture mirrors team communication, so let team structure inform boundaries.

## Related
- `../05_ACIDAndTransactionIsolation`
- `../08_CAPTheoremAndConsistencyModels`
- `../21_ObservabilityLogsMetricsTraces`
- `../29_DistributedTransactions2PCSagaOutbox`
- `../20_ServiceDiscoveryAndAPIGateway`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
