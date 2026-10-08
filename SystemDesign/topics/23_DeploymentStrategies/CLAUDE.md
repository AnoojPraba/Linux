# 23_DeploymentStrategies

Deployment strategies: blue-green, canary, rolling and feature flags, with trade-offs and rollback.

## Files
- `NOTES.md` - (39 lines) sections: Blue-green deployment; Canary deployment; Rolling deployment; Feature flags; Interview framing

## How to use this note
- Drill: pick a strategy for a database-schema-changing release vs a stateless service, and say how you detect a bad deploy.
- Distinguish deployment from release (feature flags).
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Blue-green: instant rollback by switching back, but double infrastructure during the overlap.
- Canary: small traffic share first, watch error rate/latency, ramp up or roll back; smallest blast radius.
- Rolling: replace instances gradually, no double infra, slower rollback, two versions live at once (needs compatibility).
- Feature flags decouple deploy from release and provide a kill switch.
- Observability is a prerequisite for canaries.

## Related
- `../21_ObservabilityLogsMetricsTraces`
- `../19_ResiliencePatterns`
- `../35_ContainersAndKubernetesBasics`
- `../18_MicroservicesVsMonolith`
- `../../../OS/code/44_FirmwareUpdateAndFlashStorage`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
