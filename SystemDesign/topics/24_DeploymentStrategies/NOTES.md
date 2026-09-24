# Deployment Strategies

## Blue-green deployment

- Two identical production environments; only one ("blue" or "green") is
  live at a time.
- Deploy the new version to the idle environment, test it, then switch
  traffic over. Rollback is instant - just switch back.
- Cost: double the infrastructure while both environments exist.

## Canary deployment

- Roll the new version out to a small percentage of traffic/users first,
  monitor error rates/latency (see `22_ObservabilityLogsMetricsTraces`),
  and gradually increase the percentage if healthy - or roll back
  immediately if not.
- Lower blast radius than an all-at-once deploy.

## Rolling deployment

- Gradually replace old-version instances with new-version ones a few at
  a time.
- Contrast with blue-green's all-at-once cutover: no need for double
  infrastructure, but rollback is slower (has to roll back the same way)
  and both versions run simultaneously during the rollout.

## Feature flags

- Decouple deployment from release: code for a new feature can be
  deployed "dark" (flag off) and enabled later without a new deploy.
- Enables gradual rollout, A/B testing, and an instant kill-switch if a
  feature misbehaves - no redeploy needed to turn it off.

## Interview framing

- These strategies reduce the blast radius of a bad deploy - pairs
  naturally with `20_ResiliencePatterns` (fail fast / fall back) and
  `22_ObservabilityLogsMetricsTraces` (you need metrics to know a canary
  is unhealthy before rolling it out further).
