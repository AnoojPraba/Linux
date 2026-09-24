# Microservices vs Monolith

## Monolith

- Single deployable unit - simpler to develop, test, and deploy initially.
- Transactions are easy: one database, real ACID guarantees across the
  whole app (see `05_ACIDAndTransactionIsolation`).
- Downsides: scaling requires scaling the whole app even if only one
  component is hot; codebase/team coordination gets harder as both grow.

## Microservices

- Independently deployable and independently scalable services; teams can
  own services autonomously and pick different tech stacks per service.
- Downsides:
  - Network latency between calls that used to be in-process function
    calls.
  - No more simple cross-service ACID transactions - need sagas / eventual
    consistency instead (ties into `08_CAPTheoremAndConsistencyModels`).
  - Operational overhead: more services to deploy, monitor, and secure
    (see `22_ObservabilityLogsMetricsTraces`).

## The "distributed monolith" anti-pattern

- Splitting into services without actually decoupling them: shared
  database, synchronous call chains between services, services that must
  be deployed together.
- Result: you inherit microservices' complexity (network calls,
  operational overhead) without any of the benefits (independent
  scaling/deployment).

## General guidance

- Start monolithic. Split out a service only when there's a clear
  scaling or team-ownership reason to.
- Conway's Law: a system's architecture tends to mirror the communication
  structure of the organization that built it - so team structure should
  often drive service boundaries, not the other way around.
