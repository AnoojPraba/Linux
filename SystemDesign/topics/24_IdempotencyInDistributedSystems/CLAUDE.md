# 24_IdempotencyInDistributedSystems

Why retries make idempotency necessary, the idempotency-key pattern and HTTP method idempotency.

## Files
- `NOTES.md` - (34 lines) sections: Why idempotency matters; Idempotency key pattern; HTTP methods and idempotency; Interview framing

## How to use this note
- Drill: design the idempotency-key flow for a payment POST (key storage, in-flight requests, result replay, expiry).
- Connect it to at-least-once delivery from the messaging topic.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- A retry may follow a request that already succeeded (lost response); without idempotency you double-charge or double-send.
- Idempotency key: client-generated UUID per logical operation; server stores key -> result and returns the cached result on repeat.
- GET, PUT, DELETE are naturally idempotent; POST is not, hence keys on payment-style POST APIs.
- At-least-once delivery plus an idempotent handler gives effectively-once behaviour without true exactly-once.

## Related
- `../09_MessageQueuesAndEventDrivenArchitecture`
- `../19_ResiliencePatterns`
- `../29_DistributedTransactions2PCSagaOutbox`
- `../10_APIProtocolsRESTvsGRPCvsGraphQL`
- `../../../OS/code/56_RpcMechanisms`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
