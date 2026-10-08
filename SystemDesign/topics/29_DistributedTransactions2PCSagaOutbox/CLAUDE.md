# 29_DistributedTransactions2PCSagaOutbox

Atomicity across services: two-phase commit, sagas (orchestration vs choreography, compensation), the dual-write problem and the transactional outbox, with a decision guide and senior Q&A.

## Files
- `NOTES.md` - (114 lines) sections: The problem; Two-Phase Commit (2PC); Sagas (eventual consistency with compensation); The dual-write problem and the Transactional Outbox; Related guarantees; Decision guide; Senior interviewer Q&A

## How to use this note
- Drill: walk an order/payment/inventory flow as a saga with compensations, then add the outbox for event publication.
- Use the decision-guide table to pick 2PC vs saga vs outbox vs a globally consistent database; rehearse the "Senior interviewer Q&A" answers.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- 2PC: prepare/vote then commit/abort; atomic but blocking (in-doubt participants hold locks if the coordinator dies), slow and less available; replicate the coordinator log (Spanner) to fix blocking.
- Saga: local transactions with compensating actions, no isolation (intermediate states visible), steps and compensations must be idempotent; non-compensatable steps go last (TCC: try/confirm/cancel).
- Orchestration (Temporal, Step Functions) is observable but needs a coordinator; choreography is loosely coupled but the flow is implicit.
- Outbox: write business row and event row in one local transaction, relay or CDC (Debezium) publishes at-least-once, consumers dedupe (inbox pattern).
- Exactly-once is really at-least-once plus idempotent processing; distributed locks are not transactions (fencing tokens).

## Related
- `../05_ACIDAndTransactionIsolation`
- `../22_ConsensusAndCoordination`
- `../24_IdempotencyInDistributedSystems`
- `../09_MessageQueuesAndEventDrivenArchitecture`
- `../31_StreamProcessingAndKafkaInternals`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
