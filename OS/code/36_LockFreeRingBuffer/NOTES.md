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
  `37_ConcurrentDataStructures` for other hand-built concurrency primitives
  that trade library defaults for explicit control over fairness/ordering.
