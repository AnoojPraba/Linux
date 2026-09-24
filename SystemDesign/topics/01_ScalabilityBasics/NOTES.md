# Scalability Basics

## Vertical vs horizontal scaling

- **Vertical scaling (scale up)**: add more CPU/RAM/disk to a single machine.
  Simple - no architecture changes needed - but has a hard ceiling (biggest
  box you can buy) and is a single point of failure.
- **Horizontal scaling (scale out)**: add more machines and spread load
  across them. Effectively unbounded ceiling and improves fault tolerance
  (one node dying doesn't take the whole system down), but requires the
  application to be designed for it (load balancing, no reliance on
  local-only state, coordination overhead).
- Real systems use both: scale up individual nodes to a sane cost/performance
  point, then scale out for anything beyond that.

## Stateless vs stateful services

- **Stateless service**: no per-request state is kept in server memory
  between requests - each request carries (or looks up externally) everything
  needed to handle it. Any instance can handle any request.
- **Stateful service**: holds session/connection/in-memory state tied to a
  specific server (e.g. an in-process session cache, a sticky WebSocket
  connection, in-memory game state).
- **Why statelessness enables horizontal scaling**: if any instance can serve
  any request, a load balancer can freely distribute traffic across N
  instances, and adding/removing instances is just a capacity change with no
  data-migration or routing complexity. Stateful services need sticky
  sessions, replication, or external state stores to scale out, which adds
  real complexity.
- Common pattern: push state out of the application tier into an external
  store (Redis, a database) so the application tier itself stays stateless
  and trivially horizontally scalable.

## The general scaling mindset

- Identify the actual bottleneck resource (CPU, memory, disk I/O, network,
  a specific downstream dependency) before scaling anything - scaling the
  wrong tier wastes money and doesn't fix latency/throughput.
- "Scale by adding more of the bottleneck resource" - once stateless, scaling
  a tier is just adding instances behind a load balancer; the bottleneck
  often moves downstream (e.g. to the database) once the application tier is
  no longer the limit, which is why caching/sharding/replication (later
  topics) matter as much as raw horizontal scaling.
- Interviewers want you to reason about *where* the bottleneck is and *why*
  a given scaling approach addresses it, not just say "add more servers."
