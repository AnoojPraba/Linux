# 25_DesignCaseStudyHFTOrderBookMatchingEngine

Case study: HFT order book and matching engine, where mechanical sympathy (no locks or allocation on the hot path, cache locality, single writer per instrument) matters as much as the architecture.

## Files
- `NOTES.md` - (83 lines) sections: Requirements clarification (ask before designing); Core data structure: the order book; Why HFT avoids common conveniences; Order lifecycle; Sharding by instrument; What interviewers are actually listening for

## How to use this note
- Whiteboard: order book structure, hot-path rules, order lifecycle, then sharding by instrument; say "single writer" within the first minute.
- Name concrete techniques (SPSC queue, object pools, core pinning); do not answer "use a database".
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Order book: flat array indexed by price tick for dense ranges (O(1), cache friendly), balanced tree or skip list for sparse ranges; FIFO queue per price level (price-time priority).
- Hot path: no malloc (pre-allocated pools), no locks (single-threaded matching per instrument fed by a lock-free SPSC queue).
- Cache and kernel concerns: pinned core, NUMA-aware placement, optional kernel bypass (DPDK/RDMA).
- Lifecycle: network intake, pre-trade risk checks, match against opposite side, fills/trade events, rest the remainder, publish market data on a decoupled path.
- Sharding by instrument removes cross-instrument critical sections; determinism, auditability and replayability are hard requirements alongside latency.

## Related
- `../27_DesignCaseStudyThreadSafeFixedSizeMemoryPool`
- `../26_DesignCaseStudyConcurrentInMemoryKeyValueStore`
- `../../../OS/code/37_LockFreeRingBuffer`
- `../../../OS/code/39_FalseSharing`
- `../../../C_Basics/code/51_SkipList`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.
