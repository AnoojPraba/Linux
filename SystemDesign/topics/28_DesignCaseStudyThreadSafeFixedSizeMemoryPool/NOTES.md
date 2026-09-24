# Design Case Study: Thread-Safe Fixed-Size Memory Pool

Designing a memory pool for fixed-size allocations - a systems-level case
study that complements an actual code implementation elsewhere; this note
focuses on the design discussion and tradeoffs, not a code listing.

## Requirements clarification (ask before designing)

- What allocation sizes need to be supported - a single fixed size, or a
  small set of size classes (e.g. 32B/64B/128B buckets)?
- What's the expected allocation/deallocation rate? This is usually *the*
  reason to build a custom pool in the first place - avoiding
  `malloc`/`free` overhead and fragmentation in a hot path (see
  `../26_DesignCaseStudyHFTOrderBookMatchingEngine` for a concrete
  consumer of this pattern).
- Is each pool instance used by a single thread (thread-local pools, no
  synchronization needed at all), or is one pool shared across many
  threads (needs real synchronization)?

## Core design

- Pre-allocate one large contiguous block up front, and carve it into
  fixed-size chunks.
- Maintain a free list by embedding the "next free chunk" pointer inside
  each free chunk itself - no separate metadata array is needed, since a
  free chunk isn't holding user data yet and can safely be reused to store
  the link.
- `allocate()` pops the free-list head; `deallocate()` pushes the chunk
  back onto the free-list head. Both are O(1), and the contiguous backing
  block gives good cache locality compared to chunks scattered across the
  heap.

## Thread-safety approaches, cheapest to most complex

- **Thread-local pools**: each thread owns its own pool - zero
  synchronization needed. The catch: a chunk allocated by thread A and
  freed by thread B either isn't supported, or needs a slower cross-thread
  free path (e.g. queuing the free back to the owning thread).
- **Single global free list + mutex**: simple and obviously correct, but
  serializes every allocate/deallocate call under contention - can become
  a bottleneck if the pool is on a hot path shared by many threads.
- **Lock-free free list via CAS**: push/pop the free-list head with a
  `compare_exchange` loop instead of a lock - avoids blocking, but
  introduces the ABA problem: thread A reads the head, gets preempted;
  meanwhile the chunk is popped and freed back by other threads, ending up
  back at the same address as the head; when A resumes, its CAS succeeds
  even though the free list underneath it changed. Mitigations: tagged
  pointers (pack a generation counter alongside the pointer so the CAS
  compares both), hazard pointers, or - a subtler point worth raising -
  relying on the fact that if the pool never returns memory to the OS, a
  chunk being reused doesn't corrupt the free-list linkage even when ABA
  occurs, because chunk addresses stay valid and the intrusive `next`
  pointers are self-consistent as long as no chunk is freed twice.

## What interviewers are actually listening for

- Correctly identifying *why* you'd build this instead of just calling
  `malloc`/`free`: deterministic O(1) allocation, no fragmentation (every
  chunk is the same size), and better cache locality from a contiguous
  backing block.
- The ability to reason precisely about the ABA problem when pushed on the
  lock-free variant - this is one of the most common "gotcha" follow-ups
  at this level, and hand-waving past it is a clear signal of surface-level
  knowledge.
