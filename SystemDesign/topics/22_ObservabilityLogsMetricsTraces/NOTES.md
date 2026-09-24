# Observability: Logs, Metrics, Traces

## The three pillars

- **Logs**: discrete, timestamped events. Good for detailed debugging of
  a specific incident, but expensive to query/store at scale and hard to
  correlate across services.
- **Metrics**: aggregated numeric time series (counters, gauges,
  histograms). Good for dashboards, alerting, and trend analysis; cheap
  to store long-term, but lose per-request detail.
- **Traces**: follow a single request's journey across multiple services,
  showing where time was spent. Essential once a request touches more
  than one service, since logs alone can't reconstruct the causal chain.

## Distributed tracing basics

- A trace ID is generated at the entry point and propagated across
  service boundaries (e.g. via HTTP headers).
- Each service adds a "span" covering its portion of the work; spans nest
  to form the request's call tree.
- OpenTelemetry is the current standard for instrumentation; Jaeger and
  Zipkin are common trace-visualization backends.

## The general principle

- You can't operate what you can't observe - production systems at scale
  need all three pillars, not just logs.
- Ties directly into `20_ResiliencePatterns`: circuit breaker trips and
  retry storms are only visible/debuggable if metrics and traces are in
  place; and into `19_MicroservicesVsMonolith`: the more services a
  request crosses, the more essential tracing becomes.
