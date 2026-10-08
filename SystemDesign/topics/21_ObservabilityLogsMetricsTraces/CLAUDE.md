# 21_ObservabilityLogsMetricsTraces

The three observability pillars (logs, metrics, traces) and distributed tracing basics.

## Files
- `NOTES.md` - (31 lines) sections: The three pillars; Distributed tracing basics; The general principle

## How to use this note
- Drill: given an intermittent latency complaint across five services, say which pillar you consult first and why.
- Mention trace-ID propagation and OpenTelemetry.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Logs: detailed event record, costly at scale and hard to correlate; metrics: cheap aggregated time series for dashboards and alerts but no per-request detail; traces: causal call tree for one request.
- Tracing: trace ID created at the entry point, propagated via headers, one span per service, spans nest.
- OpenTelemetry for instrumentation; Jaeger/Zipkin as backends.
- More services per request means tracing is essential; circuit-breaker trips and retry storms are only diagnosable with metrics and traces.

## Related
- `../19_ResiliencePatterns`
- `../18_MicroservicesVsMonolith`
- `../23_DeploymentStrategies`
- `../../../OS/code/71_EbpfAndTracingBasics`
- `../../../OS/code/72_PerformanceDebuggingMethodology`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
