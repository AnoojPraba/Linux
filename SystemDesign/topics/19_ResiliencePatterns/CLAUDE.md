# 19_ResiliencePatterns

Resilience patterns for calling unreliable dependencies: circuit breaker, retry with exponential backoff and jitter, bulkheads and timeouts.

## Files
- `NOTES.md` - (42 lines) sections: Circuit breaker; Retry with exponential backoff and jitter; Bulkhead isolation; Timeouts; Interview framing

## How to use this note
- Drill: walk a failing downstream through timeout, retry with jitter, breaker open/half-open and fallback.
- Mention the retry-storm failure mode unprompted.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Circuit breaker states: closed -> open (fail fast during cooldown) -> half-open (probe) -> closed or open again.
- Immediate retries create retry storms; use exponential backoff plus jitter so clients de-synchronise.
- Bulkheads partition thread/connection pools per dependency so one slow dependency cannot starve the rest.
- Every network call needs an explicit timeout or hung calls cascade upstream.
- Retries need idempotent operations (see topic 24).

## Related
- `../24_IdempotencyInDistributedSystems`
- `../09_MessageQueuesAndEventDrivenArchitecture`
- `../20_ServiceDiscoveryAndAPIGateway`
- `../12_RateLimiting`
- `../../../OS/code/73_ConcurrentTcpServers`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
