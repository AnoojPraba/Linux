# Context Switch Mechanics

- A context switch swaps out the currently-running thread/process for
  another, saving enough state to resume the first one exactly later.
- **What's saved/restored**:
  - CPU registers (general-purpose, stack pointer, program counter/
    instruction pointer, flags register).
  - FPU/SIMD state (if used) - often lazily saved to avoid cost when unused.
  - Memory management state: page table base register (e.g. CR3 on x86) -
    only needed when switching between different processes, not threads of
    the same process, since threads share an address space.
  - Kernel-level bookkeeping: scheduling info, signal masks, etc.
- **Voluntary vs involuntary switch**: voluntary happens when a
  thread blocks itself (I/O wait, `sched_yield()`, mutex wait); involuntary
  happens when the scheduler preempts a running thread (timeslice expired,
  higher-priority thread became ready).
- **Cost sources**:
  - Direct: saving/restoring register state (cheap, mostly a few
    instructions).
  - Indirect (usually dominant): cache and TLB pollution - the new
    thread/process starts with cold L1/L2 caches and (for cross-process
    switches) a flushed/re-tagged TLB, causing a burst of cache/TLB misses
    as it "warms back up".
  - Cross-process switches cost more than cross-thread switches (same
    process) because of the page-table/TLB overhead threads avoid by
    sharing an address space.
- **Empirical demo**: `01_yieldTiming.c` times many `sched_yield()` calls;
  dividing total time by call count gives a rough per-switch cost (typically
  hundreds of nanoseconds to low microseconds on modern Linux, though this
  varies heavily by system load and core count).
- Interview framing: "why are threads cheaper than processes?" -> shared
  address space means thread context switches skip the page-table/TLB
  invalidation cost that process switches pay.
