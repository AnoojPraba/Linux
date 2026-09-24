# Design Case Study: HFT Order Book / Matching Engine

Designing a high-frequency-trading matching engine - the case study where
low-level performance concerns (cache locality, lock-free structures,
mechanical sympathy) matter as much as the high-level architecture.

## Requirements clarification (ask before designing)

- What asset class - equities, futures, crypto? (Affects trading hours,
  tick size, regulatory constraints.)
- What matching algorithm - price-time priority (FIFO within a price
  level) is the standard default; pro-rata matching exists for some
  markets but is less common to be asked about.
- What's the latency budget? HFT typically means single-digit to
  low-double-digit microseconds end to end (wire-to-wire), which rules out
  anything involving locks or allocation on the hot path.
- Single instrument or many? (A single instrument's book is a much simpler
  problem than a multi-instrument exchange - see sharding note below.)
- Which order types are in scope beyond basic limit/market - stop orders,
  iceberg orders, IOC/FOK?

## Core data structure: the order book

- A price-level map: for a bounded/active price range, a flat array
  indexed by price tick is fastest (O(1) access, great cache locality);
  for a wide or sparse range, a balanced tree or skip list is more
  practical. Each price level holds a FIFO queue of resting orders (time
  priority within the level).
- This is the same tree-vs-array-vs-skip-list tradeoff covered generally
  for ordered data structures - here the deciding factor is the price
  range's density, not just theoretical big-O.

## Why HFT avoids common conveniences

- **No dynamic allocation in the hot path**: orders and book nodes come
  from pre-allocated, fixed-size object pools, not `malloc`/`new` per
  order (see `../28_DesignCaseStudyThreadSafeFixedSizeMemoryPool` for the
  pool design itself).
- **No locks in the hot path**: a single-threaded matching engine per
  instrument is a common design - since only one thread ever mutates a
  given instrument's book, there's no contention to synchronize away.
  Order intake threads hand orders to the matching thread over a
  lock-free single-producer/single-consumer queue.
- **Minimizing cache misses**: data structures are laid out for cache
  locality, the matching thread is pinned to a dedicated core (NUMA-aware
  placement), and the most extreme designs use kernel-bypass networking
  (DPDK/RDMA) to avoid syscall and context-switch overhead entirely.

## Order lifecycle

1. Order arrives over the network.
2. Pre-trade risk checks (position limits, fat-finger checks) - ideally
   fast enough not to dominate the latency budget.
3. The matching engine attempts to match against the opposite side: a buy
   matches the lowest-priced resting sell at or below its limit price (and
   symmetrically for sells).
4. A full or partial fill generates trade events; any unmatched remainder
   rests in the book at its price level.
5. Market data (book updates) and trade confirmations are published via a
   separate, decoupled path so publishing never blocks the matching logic
   itself.

## Sharding by instrument

- Since each instrument's book only needs one writer, sharding the
  engine by instrument (one single-threaded matching engine per
  instrument, or per small group of instruments) sidesteps almost all
  concurrency complexity - there's no cross-instrument critical section to
  protect. This "single-writer" framing is the single most important
  insight in this case study.

## What interviewers are actually listening for

- Recognizing that correctness (deterministic, auditable, replayable
  matching) and latency are both hard requirements, and that some
  techniques trade one for the other.
- The single-writer-per-instrument insight - that sharding by instrument
  eliminates most of the concurrency problem, rather than needing clever
  locking.
- Naming concrete techniques (lock-free SPSC queues, pre-allocated object
  pools, core pinning) instead of hand-waving toward "use a database" or
  "just use a mutex" - this is a domain where implementation details are
  the point of the question.
