# 28_CommonInterviewQuestionsCheatSheet

Question bank of quick-fire system design questions by category (not a design walkthrough), with 1-3 sentence answers and pointers to the full topics.

## Files
- `NOTES.md` - (145 lines) sections: Purpose; 1. Clarifying-question discipline; 2. Scaling/availability tradeoff questions; 3. Consistency/correctness questions; 4. "Why not just..." challenge questions; 5. Capacity estimation quick-fire questions; 6. Meta questions about the interview itself

## How to use this note
- Drill each category aloud in 1-3 sentences; use it as a checklist of angles while practising a full design.
- Categories: clarifying questions, scaling/availability trade-offs, consistency/correctness, "why not just..." challenges, capacity quick-fire, meta questions about the interview.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Always clarify scale, read:write ratio, latency budget and consistency need before designing.
- DB bottleneck order: read replicas, caching, query/index tuning, vertical scaling, denormalisation, sharding last.
- Payment retries: idempotency keys; locks: leases with TTL plus fencing tokens; split-brain: consensus plus quorum.
- "Why not just cache everything / use strong consistency / add servers" each has a cost to articulate.
- Senior signal: surface trade-offs, failure modes and operations (monitoring, rollout, on-call) unprompted; do not name a technology without saying why it is needed.

## Gotchas
- One answer here is out of date: the CRDT/OT question says this repo does not cover CRDT/OT internals in depth, but `../32_CausalityVectorClocksAndCRDTs` now exists.

## Related
- `../17_CapacityEstimationAndBackOfEnvelopeMath`
- `../13_DesignCaseStudyURLShortener`
- `../08_CAPTheoremAndConsistencyModels`
- `../24_IdempotencyInDistributedSystems`
- `../../../Behavioral/topics/01_BehavioralAndLeadershipInterviewPrep`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
