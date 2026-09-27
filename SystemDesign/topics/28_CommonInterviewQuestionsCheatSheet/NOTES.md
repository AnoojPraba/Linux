# Common System Design Interview Questions Cheat Sheet

## Purpose

- This is a QUESTION BANK, not a design walkthrough - quick-fire questions you should
  be able to discuss in 1-3 sentences, organized by category. Contrast with the full
  end-to-end designs in `13_DesignCaseStudyURLShortener`, `14_DesignCaseStudyChatSystem`,
  `15_DesignCaseStudyDistributedCache`, `25_DesignCaseStudyHFTOrderBookMatchingEngine`,
  `26_DesignCaseStudyConcurrentInMemoryKeyValueStore`, and
  `27_DesignCaseStudyThreadSafeFixedSizeMemoryPool`.
- Use this list to drill yourself before an interview, or as a checklist of "did I
  cover this angle" while practicing a full design.

## 1. Clarifying-question discipline

- **Before designing anything, what clarifying questions should you always ask?**
  Scale (DAU/QPS), read:write ratio, latency budget, consistency requirements, and
  whether there's existing infra to integrate with. See
  `17_CapacityEstimationAndBackOfEnvelopeMath`.
- **How do you avoid over-scoping a 45-minute interview?** State assumptions, pick a
  narrow initial scope (e.g. "text-only chat, ignore media for now"), and explicitly
  flag what you're deferring.
- **What's the latency budget for this system, and why does it matter early?** It
  determines whether you need caching/CDN/edge compute or a simple DB-backed service.
- **Is this a read-heavy or write-heavy system, and how does that change your first
  instinct on architecture?** Read-heavy pushes toward caching/replicas; write-heavy
  pushes toward sharding/queueing.
- **What consistency guarantee does the product actually need?** Don't default to
  "strong consistency everywhere" - ask if eventual consistency is acceptable for the
  use case (see `08_CAPTheoremAndConsistencyModels`).
- **What's already there?** Existing message queue, existing auth service, existing
  data store - reusing infra changes the design more than any clever component choice.

## 2. Scaling/availability tradeoff questions

- **How would you scale a system that suddenly gets 100x more read traffic
  overnight?** Add read replicas, put a cache in front, and push static/cacheable
  content to a CDN before considering re-architecting. See `03_CachingStrategies`,
  `07_DatabaseShardingAndReplication`.
- **Your database is the bottleneck - what are your options, in rough order you'd
  consider them?** Read replicas -> caching -> query/index optimization -> vertical
  scaling -> denormalization -> sharding. Sharding is the most invasive, so it's last.
- **How do you keep a system available during a rolling deployment?** Health checks,
  gradual traffic shift, and backward-compatible schema/API changes. See
  `23_DeploymentStrategies`.
- **What happens to your system during a network partition, and which side of CAP
  does your design choose?** Say explicitly whether you favor availability or
  consistency for the specific use case, not "both." See
  `08_CAPTheoremAndConsistencyModels`.
- **How do you scale writes when a single leader DB can't keep up?** Shard by a key
  with even distribution, or move to an async queue plus eventual persistence. See
  `07_DatabaseShardingAndReplication`, `09_MessageQueuesAndEventDrivenArchitecture`.
- **What's a single point of failure in your design, and how would you remove it?**
  Every component should have this question asked of it - load balancer, DB leader,
  cache node, etc.

## 3. Consistency/correctness questions

- **How would you prevent double-charging a customer if your payment API call times
  out but may have actually succeeded?** Idempotency keys on the client request so a
  retry is a no-op server-side. See `24_IdempotencyInDistributedSystems`.
- **Two users edit the same document at the same time - how do you handle the
  conflict?** At a discussion level: last-write-wins is simple but lossy,
  operational-transform/CRDT-style merging preserves both edits but adds real
  complexity - this repo doesn't cover CRDT/OT internals in depth, so name the
  tradeoff rather than the algorithm.
- **How would you design a system where a distributed lock is required, and what
  happens if the lock holder crashes without releasing it?** Use a lock with a TTL/
  lease so it auto-expires, plus fencing tokens to prevent a stale holder from acting
  after expiry. See `22_ConsensusAndCoordination`.
- **How do you make a retried request safe to process twice?** Same answer as
  payments generally - idempotency keys, or design the operation itself to be
  naturally idempotent (e.g. "set counter to 5" instead of "increment by 1").
- **How would you detect and resolve a split-brain scenario?** Use a consensus
  protocol (e.g. Raft) so only one node can hold leadership at a time, backed by a
  quorum. See `22_ConsensusAndCoordination`.
- **When is eventual consistency actually fine to ship?** When stale reads are
  cosmetic and self-correct quickly (e.g. like counts, follower counts) rather than
  affecting money or safety-critical logic.

## 4. "Why not just..." challenge questions

- **Why not just use a bigger database server instead of sharding?** Vertical
  scaling has a hard ceiling, costs grow non-linearly, and it's still a single point
  of failure. See `01_ScalabilityBasics`, `07_DatabaseShardingAndReplication`.
- **Why not just cache everything?** Staleness risk, cache invalidation complexity,
  and memory cost all grow - caching is a targeted tool for hot/read-heavy data, not
  a default. See `03_CachingStrategies`.
- **Why not just make every service call synchronous for simplicity?** Latency
  stacks across the call chain and a slow/failing downstream call cascades into
  every caller - use async/queues and circuit breakers instead. See
  `19_ResiliencePatterns`, `09_MessageQueuesAndEventDrivenArchitecture`.
- **Why not just use strong consistency everywhere to avoid bugs?** It costs
  availability and latency under partition, and most product flows don't actually
  need it - reserve it for the few operations where correctness trumps speed. See
  `08_CAPTheoremAndConsistencyModels`.
- **Why not just add more servers behind a load balancer for every bottleneck?**
  Horizontal scaling doesn't help a stateful bottleneck (e.g. a single DB leader) or
  a problem that's actually about hot-key contention rather than raw capacity.
- **Why not just poll instead of using a message queue?** Polling wastes resources
  and adds latency proportional to poll interval; a queue/pub-sub model decouples
  producer and consumer and delivers near-real-time. See
  `09_MessageQueuesAndEventDrivenArchitecture`.

## 5. Capacity estimation quick-fire questions

- **Estimate the storage needed for a photo-sharing app with N daily active users
  uploading M photos/day at average size S.** Daily storage = N x M x S; multiply by
  retention period and replication factor for total footprint.
- **Estimate the read QPS for a URL shortener given X redirects/day.** Average QPS =
  X / 86,400; multiply by 2-3x for peak QPS.
- **Estimate how many servers you'd need to handle Y requests/second at Z ms average
  latency per server.** Servers ~= Y x (Z / 1000) / requests-handled-concurrently-
  per-server, then add headroom for peak and failover.
- **Estimate the bandwidth needed for a video-streaming service with K concurrent
  viewers at bitrate B.** Bandwidth = K x B; compare against a single server's NIC
  capacity to decide fleet size.
- **Estimate the write QPS for a chat app with P active users sending an average of
  A messages/day.** Average write QPS = (P x A) / 86,400, split further if group
  chats fan out to multiple recipients.
- Full methodology and reference numbers live in
  `17_CapacityEstimationAndBackOfEnvelopeMath` - use these prompts to drill the
  process until it's fast and automatic.

## 6. Meta questions about the interview itself

- **What is the interviewer actually evaluating in a system design interview,
  beyond whether your design "works"?** Structured thinking, ability to articulate
  tradeoffs (not just name components), going deeper when pushed, and clear
  communication under ambiguity.
- **How should you handle a question you don't know the answer to?** Reason from
  first principles out loud rather than guessing silently or bluffing - interviewers
  weight process over a lucky right answer.
- **What's a red flag that tells an interviewer you're pattern-matching instead of
  understanding?** Naming a technology (e.g. "we'll use Kafka") without being able
  to say why it's needed here or what breaks without it.
- **How do you show senior-level thinking in a design interview?** Proactively
  surface tradeoffs, failure modes, and operational concerns (monitoring, rollout,
  on-call) without being prompted - see the `Behavioral` repo's
  `01_BehavioralAndLeadershipInterviewPrep` topic for how this pairs with
  leadership narrative in the behavioral portion.
- **How should you react when the interviewer pushes back hard on a choice?**
  Treat it as a chance to go deeper, not a sign you were wrong - restate the
  tradeoff, and change your answer only if the pushback reveals a real requirement
  you missed.
