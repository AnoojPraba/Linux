# 17_CapacityEstimationAndBackOfEnvelopeMath

Back-of-envelope estimation: QPS, storage and bandwidth math with handy numbers, framed as a way to justify architecture decisions.

## Files
- `NOTES.md` - (52 lines) sections: Why it matters; Handy reference numbers; QPS estimation; Storage estimation; Bandwidth estimation; Interview framing

## How to use this note
- Drill: estimate QPS, peak QPS, storage with replication and bandwidth for a feed or URL shortener in under three minutes, aloud.
- Always finish with the decision the numbers imply (shard or not, cache or not).
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Handy numbers: 1 day ~ 10^5 s, million = 10^6, billion = 10^9; read:write often 100:1 to 1000:1.
- Average QPS = DAU x requests per user per day / 86,400; peak = 2-3x average; split into read and write QPS.
- Storage = record size x records x retention x replication factor (often 3x).
- Bandwidth = QPS x payload, separately for ingress and egress; compare with link capacity.
- Round aggressively: order of magnitude matters, precision does not.

## Related
- `../13_DesignCaseStudyURLShortener`
- `../14_DesignCaseStudyChatSystem`
- `../15_DesignCaseStudyDistributedCache`
- `../01_ScalabilityBasics`
- `../28_CommonInterviewQuestionsCheatSheet`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
