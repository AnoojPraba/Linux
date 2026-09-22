# Cache Coherence: MESI Protocol

Notes-only - this ties directly into `62_FalseSharing`, which already has a
runnable demo of the *symptom* (cache-line ping-pong slowing down
"unrelated" per-thread counters). This folder covers the *mechanism*
underneath that demo instead of duplicating it.

## Why coherence is needed

- Each CPU core has its own private cache(s); when multiple cores cache the
  same memory address (same cache line), writes by one core must become
  visible to the others in a consistent order - otherwise cores could
  disagree about a variable's value indefinitely.

## MESI states (per cache line, per core)

- **Modified (M)**: this core has the only cached copy, and it has been
  written (dirty) - differs from memory. Must write back before another core
  can read it.
- **Exclusive (E)**: this core has the only cached copy, and it matches
  memory (clean). Can silently transition to Modified on a local write with
  no bus traffic, since no other core has a copy to invalidate.
- **Shared (S)**: this line is cached by this core and potentially others,
  all holding the same (clean) value.
- **Invalid (I)**: this core's copy is stale/absent - must fetch from memory
  or another core before use.

## Typical transitions

- Core reads an address no one else has cached -> loads it as **Exclusive**.
- Another core also reads it -> both go to **Shared**.
- A core writes to a Shared line -> broadcasts an invalidate; other cores'
  copies go to **Invalid**; the writer's line becomes **Modified**.
- A core reads a line another core holds Modified -> the modified data is
  flushed (write-back or cache-to-cache transfer) and both may end up
  **Shared** (or the reader gets it and the original goes to Invalid,
  depending on implementation).

## Why this matters for concurrent code

- Coherence guarantees correctness (every core eventually sees writes) but
  the invalidate/re-fetch traffic on every transition is NOT free - it's
  exactly what makes false sharing slow: two cores writing to *different*
  variables that happen to share a cache line still ping-pong that line
  between Modified/Invalid on every write, even though there's no logical
  data race. See `62_FalseSharing/01_falseSharingBenchmark.c` for the
  measured cost of this.
- Cache-line size (commonly 64 bytes on x86) is the granularity at which all
  of this happens - coherence operates on whole lines, not individual bytes.
