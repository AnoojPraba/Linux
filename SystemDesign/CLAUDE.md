# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository purpose

This is a personal System Design / architecture interview-prep repository for a
15-years-experience candidate - covering scalability fundamentals (horizontal vs
vertical scaling, statelessness), load balancing, caching, database internals
(indexing, transactions/ACID, SQL vs NoSQL, sharding/replication), distributed-systems
theory (CAP theorem, consistency models), messaging/event-driven architecture, API
protocol tradeoffs (REST/gRPC/GraphQL), networking basics (DNS/TLS), rate limiting,
end-to-end design case studies (URL shortener, chat system, distributed cache), security
fundamentals, and behavioral/leadership interview prep. Unlike the sibling `../C_Basics/`,
`../Cpp/`, and `../OS/` repos, this material is almost entirely whiteboard/conceptual -
there is no runnable code, no build system, and no test suite. Each topic is a
`NOTES.md` writeup; a code snippet or ASCII-art diagram is included only where it's
genuinely illustrative (most topics won't have one).

## Structure

- `topics/` - numbered topic folders (`01_ScalabilityBasics` through
  `25_IdempotencyInDistributedSystems`) in foundational -> advanced -> case-study ->
  soft-skills order, each holding a `NOTES.md`: `01_ScalabilityBasics`,
  `02_LoadBalancing`, `03_CachingStrategies`, `04_DatabaseIndexingAndQueryOptimization`,
  `05_ACIDAndTransactionIsolation`, `06_SQLvsNoSQLTradeoffs`,
  `07_DatabaseShardingAndReplication`, `08_CAPTheoremAndConsistencyModels`,
  `09_MessageQueuesAndEventDrivenArchitecture`, `10_APIProtocolsRESTvsGRPCvsGraphQL`,
  `11_DNSAndTLSBasics`, `12_RateLimiting`, `13_DesignCaseStudyURLShortener`,
  `14_DesignCaseStudyChatSystem`, `15_DesignCaseStudyDistributedCache`,
  `16_SecurityFundamentalsForInterviews`, `17_BehavioralAndLeadershipInterviewPrep`,
  `18_CapacityEstimationAndBackOfEnvelopeMath`, `19_MicroservicesVsMonolith`,
  `20_ResiliencePatterns`, `21_ServiceDiscoveryAndAPIGateway`,
  `22_ObservabilityLogsMetricsTraces`, `23_ConsensusAndCoordination`,
  `24_DeploymentStrategies`, `25_IdempotencyInDistributedSystems`,
  `26_DesignCaseStudyHFTOrderBookMatchingEngine`,
  `27_DesignCaseStudyConcurrentInMemoryKeyValueStore`,
  `28_DesignCaseStudyThreadSafeFixedSizeMemoryPool`.
  Topics `01`-`08` are the distributed-systems/database foundations; `09`-`12` cover
  messaging, API protocol choices, networking, and rate limiting; `13`-`15` are
  end-to-end design case studies that draw on all the earlier topics; `16` covers
  security fundamentals; `17` is behavioral/leadership interview prep, distinct from the
  technical topics but expected at this experience level; `18`-`25` are additional
  interview-prep coverage areas added later - capacity estimation/back-of-envelope math,
  microservices vs monolith, resilience patterns (circuit breakers/retries/bulkheads),
  service discovery and API gateways, observability (logs/metrics/traces), distributed
  consensus and coordination, deployment strategies, and idempotency in distributed
  systems; `26`-`28` are further low-level-systems-design case studies in the same
  vein as `13`-`15` - an HFT order book/matching engine, a concurrent in-memory
  key-value store, and a thread-safe fixed-size memory pool - that lean more heavily
  on mechanical-sympathy/concurrency-primitive concerns than the earlier case studies.
  `09_MessageQueuesAndEventDrivenArchitecture` and `10_APIProtocolsRESTvsGRPCvsGraphQL`
  cross-reference the RPC/delivery-semantics material in `../OS/code/51_RpcMechanisms`
  and `../Cpp/code/31_RpcMechanismsCpp`.

## Working with this codebase

- There is no build system, Makefile, or compiler here - this repo is conceptual notes
  only, not code. Nothing under `topics/` is meant to be compiled or run.
- Each `NOTES.md` is a concise, bullet-point, interview-focused writeup, not essay prose -
  matching the tone of the sibling repos' `NOTES.md` files.
- New topics should follow the existing naming pattern: a two-digit numeric prefix
  followed by a short descriptive folder name (e.g. `18_NextTopicName`), placed in
  foundational -> advanced -> case-study -> soft-skills order alongside the existing
  folders.
- Core C/C++/OS-internals topics remain in the sibling `../C_Basics/`, `../Cpp/`, and
  `../OS/` repos; this repo is exclusively for system-design/architecture/behavioral
  interview prep.
