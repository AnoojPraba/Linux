# Lock-Free Ring Buffer

- Both files implement a bounded circular queue without any mutex - progress
  is guaranteed by atomics rather than mutual exclusion.

## SPSC (`01_spscRingBuffer.c`)

- Exactly one thread ever writes `head`, exactly one ever writes `tail` -
  each index has a single owner, so plain atomic loads/stores with
  acquire/release ordering are enough. No compare-and-swap is needed because
  there is never a race to *claim* a slot - only a race to *observe* the
  other side's index.

## MPMC (`02_mpmcRingBuffer.c`)

- Multiple producers race on the same shared write index, and multiple
  consumers race on the same shared read index - a plain load-then-store is
  no longer safe, so both sides use a `compare_exchange_weak` loop to
  atomically claim a slot before touching it (Vyukov's design).
- Each slot carries its own sequence number instead of relying solely on the
  head/tail indices. The sequence tells a thread whether *this specific
  slot* is currently ready for write or ready for read, and by how much the
  ring has "lapped" (wrapped around) - without it, two producers could both
  believe they'd claimed the same physical slot on different laps.
- Contrast: SPSC pays a fraction of the synchronization cost of MPMC because
  it has no contention to resolve, only visibility to guarantee.
- See `21_AdvancedSyncPrimitives/03_fairRwLockFromScratch.c` and
  `38_ConcurrentDataStructures` for other hand-built concurrency primitives
  that trade library defaults for explicit control over fairness/ordering.

## Senior interviewer Q&A
**Q: Why is the SPSC ring buffer correct with only loads and stores (no CAS)?**
A: Each index has exactly one writer: the producer owns `tail`, the consumer owns
`head`. Each side only READS the other's index to decide full/empty, so there is
nothing to race over. What is needed is *visibility ordering*: the producer must
write the slot BEFORE publishing the new `tail` (release), and the consumer must read
`tail` (acquire) BEFORE reading the slot - otherwise it could see the new tail but
stale slot data.

**Q: The demo uses default `atomic_load/store` (seq_cst). How would you improve it?**
A: Weaken to the minimum: producer `store_release(tail)`, `load_acquire(head)`
(or relaxed for its own index); consumer mirrors that. On x86 this compiles to plain
`mov`s; on ARM it avoids full barriers. Verify with TSan and a litmus-style stress
test on ARM (`../66_MemoryModelLitmusTests`).

**Q: What performance bug does a struct with adjacent `head` and `tail` have?**
A: **False sharing**: the producer writes `tail` and the consumer writes `head`, both
on one 64-byte cache line, so the line ping-pongs between cores on every operation
even though no data is logically shared. Fix: `_Alignas(64)` (or padding) per index,
and often cache a local copy of the other side's index to avoid reading it every time
(`../39_FalseSharing`, `../40_CacheCoherenceMESI`).

**Q: Why does this ring keep one slot empty?**
A: `head == tail` must mean empty, so "full" is `(tail+1) % N == head`; the sacrificed
slot disambiguates the two. Alternatives: monotonically increasing 64-bit counters
(mask with `N-1` for power-of-two sizes; full = `tail - head == N`) which use every
slot and avoid the modulo - the common production design.

**Q: Why power-of-two capacity?**
A: Replace `% N` (a division) with `& (N-1)`, and with free-running counters
wraparound is natural; very cheap indexing.

**Q: What does the sequence number per slot do in Vyukov's MPMC queue?**
A: It tells a thread whether THIS slot is ready for a producer (`seq == pos`) or
consumer (`seq == pos + 1`) on this lap, so claiming a position via CAS and
then using the slot cannot collide with a thread one lap ahead/behind. It also
carries the release/acquire publish of the slot's data.

**Q: Is MPMC faster than a mutex-protected queue?**
A: Not always. Under high contention CAS retries and cache-line bouncing on the
shared indices limit it; under low contention a mutex is fine. The real win is
no blocking/priority inversion/convoying - and predictable latency. Measure; also
consider sharded SPSC queues (one per producer) merged by a consumer.

**Q: How do you wait when the queue is empty/full - spin or block?**
A: Spinning burns CPU; use bounded spin + `sched_yield`/pause, then park on a
futex/eventfd/condvar (`../65_FutexAndSeqlock`). Lock-free data structure + blocking
wakeup layer is the typical production design.

**Q: Where are ring buffers used?**
A: NIC RX/TX descriptor rings, io_uring SQ/CQ, audio pipelines, logging
(LMAX Disruptor), inter-core messaging, kernel `kfifo`, trace buffers.

**Q: How would you test it?**
A: Stress with many iterations and an invariant (sum/sequence check, no loss,
no duplicates), ThreadSanitizer, ARM hardware or a model checker, fault injection
with random delays, and checking behavior on full/empty/wrap boundaries.
