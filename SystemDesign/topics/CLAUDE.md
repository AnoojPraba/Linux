# SystemDesign/topics

Index of the 35 numbered system-design topic folders (`01_ScalabilityBasics` to `35_ContainersAndKubernetesBasics`). Conceptual only: each folder holds one `NOTES.md` and its own CLAUDE.md (sections, key trade-offs, related topics).

## How to use (no build)
- There is no code, Makefile or test suite here; nothing to compile.
- Practise by whiteboarding the design aloud, then compare with the NOTES. Answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs.
- Case studies lean on the foundation topics: do `17` (estimation), `03` (caching), `07` (sharding/replication) and `08` (CAP) first.
- NOTES.md style is concise bullets; new topics take the next prefix (`36_...`).

## Numbering and layout
- `01`-`08`: scalability, load balancing, caching, indexing, transactions, SQL vs NoSQL, sharding/replication, CAP.
- `09`-`12`: messaging, API protocols, DNS/TLS, rate limiting. `13`-`15`: case studies (URL shortener, chat, distributed cache). `16`: security.
- `17`-`24`: estimation, microservices, resilience, discovery/gateway, observability, consensus, deployment, idempotency. `25`-`27`: low-level case studies (HFT matching engine, concurrent KV store, memory pool). `28`: question bank.
- `29`-`35` (added later): distributed transactions, LSM trees, Kafka/streaming, causality/CRDTs, news feed, search, Kubernetes.
- `29`-`35` end NOTES.md with a "Senior interviewer Q&A" section.
- `01_ScalabilityBasics` and `02_LoadBalancing` contain untracked vim swap files (`.NOTES.md.swp`); they are not content.
- Cross-domain: C/OS/C++ code that backs these topics is linked from each folder's Related section (e.g. `../../C_Basics/code/37_LRUCache`, `../../OS/code/37_LockFreeRingBuffer`).

## Folder map
| Folder | Purpose |
|---|---|
| `01_ScalabilityBasics` | Foundation note: vertical vs horizontal scaling, stateless vs stateful services and the bottleneck-first scaling mindset |
| `02_LoadBalancing` | L4 vs L7 load balancing, balancing algorithms (round robin, least connections, consistent hashing) and liveness vs... |
| `03_CachingStrategies` | Where caches live (CDN, in-memory), invalidation strategies (TTL, cache-aside, write-through, write-back) and stampede... |
| `04_DatabaseIndexingAndQueryOptimization` | Database indexing and query tuning: B-tree vs hash indexes, composite index column order, N+1 queries, reading EXPLAIN... |
| `05_ACIDAndTransactionIsolation` | ACID properties, the four SQL isolation levels with the anomalies each prevents |
| `06_SQLvsNoSQLTradeoffs` | When relational wins vs when NoSQL wins, the four NoSQL families with an example use each |
| `07_DatabaseShardingAndReplication` | Sharding strategies (range, hash, geographic), hot shards, leader-follower replication, replication lag and sync vs... |
| `08_CAPTheoremAndConsistencyModels` | CAP theorem (really CP vs AP under partitions), strong vs eventual consistency and quorum reads/writes (W + R > N) |
| `09_MessageQueuesAndEventDrivenArchitecture` | Async decoupling with message queues: delivery semantics, pub/sub vs point-to-point, backpressure, dead-letter queues |
| `10_APIProtocolsRESTvsGRPCvsGraphQL` | REST vs gRPC vs GraphQL: strengths, weaknesses and where each fits |
| `11_DNSAndTLSBasics` | DNS resolution chain with caching and TLS handshake basics (certificates, key exchange, why symmetric crypto for bulk... |
| `12_RateLimiting` | Rate-limiting algorithms (token bucket, leaky bucket, fixed and sliding window) and distributed enforcement |
| `13_DesignCaseStudyURLShortener` | Case study: URL shortener (requirements questions, ID generation approaches, schema and caching, what interviewers... |
| `14_DesignCaseStudyChatSystem` | Case study: chat system (WebSockets vs polling, ordering and presence, history storage, group fan-out, offline delivery) |
| `15_DesignCaseStudyDistributedCache` | Case study: distributed cache cluster (consistent hashing with virtual nodes, replication, eviction, Dynamo-style... |
| `16_SecurityFundamentalsForInterviews` | Interview-level security: OWASP Top 10 highlights, authentication vs authorization, session vs JWT auth, OAuth2... |
| `17_CapacityEstimationAndBackOfEnvelopeMath` | Back-of-envelope estimation: QPS, storage and bandwidth math with handy numbers, framed as a way to justify... |
| `18_MicroservicesVsMonolith` | Monolith vs microservices trade-offs, the distributed-monolith anti-pattern and Conway's Law |
| `19_ResiliencePatterns` | Resilience patterns for calling unreliable dependencies: circuit breaker, retry with exponential backoff and jitter,... |
| `20_ServiceDiscoveryAndAPIGateway` | Service discovery (client-side vs server-side) and the API gateway pattern with its cross-cutting concerns and... |
| `21_ObservabilityLogsMetricsTraces` | The three observability pillars (logs, metrics, traces) and distributed tracing basics |
| `22_ConsensusAndCoordination` | Consensus (Paxos/Raft at a conceptual level), leader election and distributed locking, including the Redlock caveat |
| `23_DeploymentStrategies` | Deployment strategies: blue-green, canary, rolling and feature flags |
| `24_IdempotencyInDistributedSystems` | Why retries make idempotency necessary, the idempotency-key pattern and HTTP method idempotency |
| `25_DesignCaseStudyHFTOrderBookMatchingEngine` | Case study: HFT order book and matching engine, where mechanical sympathy (no locks or allocation on the hot path,... |
| `26_DesignCaseStudyConcurrentInMemoryKeyValueStore` | Case study: single-node Redis-like in-memory key-value store |
| `27_DesignCaseStudyThreadSafeFixedSizeMemoryPool` | Case study: thread-safe fixed-size memory pool (intrusive free list, thread-local vs mutex vs lock-free CAS, the ABA... |
| `28_CommonInterviewQuestionsCheatSheet` | Question bank of quick-fire system design questions by category (not a design walkthrough) |
| `29_DistributedTransactions2PCSagaOutbox` | Atomicity across services: two-phase commit, sagas (orchestration vs choreography, compensation), the dual-write... |
| `30_LSMTreesAndStorageEngines` | Storage-engine internals: B-tree (update-in-place) vs LSM-tree (log-structured merge), write/read path, compaction and... |
| `31_StreamProcessingAndKafkaInternals` | Kafka as a partitioned replicated commit log (storage, replication/ISR, producers, consumer groups, delivery semantics)... |
| `32_CausalityVectorClocksAndCRDTs` | Ordering without a global clock: happens-before, Lamport and vector clocks, HLC/TrueTime, conflict-handling strategies... |
| `33_DesignCaseStudyNewsFeed` | Case study: news feed / timeline (requirements, estimates, architecture, fan-out on write vs read vs hybrid, data... |
| `34_DesignCaseStudySearchAndInvertedIndex` | Case study: full-text search built on an inverted index (analysis, postings, ranking with BM25, Elasticsearch-style... |
| `35_ContainersAndKubernetesBasics` | Kubernetes for system design: control plane and nodes, reconciliation loop, core objects, scheduling and resources,... |
