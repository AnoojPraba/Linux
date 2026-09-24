# Swap, Thrashing, and the Working-Set Model

- **Swap space**: reserved disk (or SSD) space the OS uses as an overflow
  for physical RAM. When memory pressure is high, the kernel evicts
  infrequently-used pages from RAM to swap, freeing frames for active pages;
  a later access to a swapped-out page triggers a major page fault
  (`32_VirtualMemoryDeepDive`) that reads it back in. Swap lets the system
  keep running (slowly) past its physical RAM limit instead of failing
  allocations outright, at the cost of disk-speed latency on a swap-in.

## Page replacement algorithms - survey

  Deciding *which* resident page to evict when a new page must be brought
  in and no frame is free:
  - **FIFO** (First-In-First-Out) and **LRU** (Least Recently Used) are
    already implemented and compared head-to-head in
    `28_MemoryManagement/01_pageReplacement.c` - see that file for the
    working code and per-algorithm tradeoff notes. Summary: FIFO evicts the
    oldest-loaded page regardless of use; LRU evicts the page unused for
    the longest time, needing per-page recency tracking.
  - **Optimal (Belady's algorithm)**: evict the page that will not be used
    for the longest time *in the future* - provably the minimum possible
    number of faults for any reference string. Only computable with full
    foreknowledge of future references, so it's never usable online; it
    exists purely as a theoretical lower-bound/benchmark to measure real
    algorithms against. Demoed in `01_optimalPageReplacement.c` in this
    folder (not covered elsewhere in this repo).
  - **Second-Chance / Clock**: an approximation of LRU that's cheap in
    hardware. Frames are arranged in a circular list with a "hand" pointer
    and each page has a reference bit (set by hardware on access). On a
    fault, the hand advances; if the pointed-to page's reference bit is set,
    it's cleared and given a "second chance" (skipped); if clear, that page
    is evicted. Approximates LRU without needing a full recency timestamp
    per page. Demoed in `02_clockPageReplacement.c` in this folder (not
    covered elsewhere in this repo).

## Belady's Anomaly

- Intuitively, giving an algorithm *more* frames should never increase its
  page fault count - more memory should only help. **Belady's Anomaly** is
  the counterexample: for some reference strings, **FIFO** can produce
  *more* faults with 4 frames than with 3. `03_beladysAnomaly.c` reproduces
  this on the standard textbook reference string `1,2,3,4,1,2,5,1,2,3,4,5`.
- **Why FIFO is vulnerable**: FIFO is not a "stack algorithm" - the set of
  pages resident with N frames is not guaranteed to be a subset of the set
  resident with N+1 frames at every point in the reference string, because
  eviction order depends only on load time, not on which pages are actually
  still useful. Adding a frame can change *which* page gets evicted at an
  earlier step in a way that, counterintuitively, causes an extra fault
  later on.
- **LRU and Optimal are stack algorithms** - they can be shown to always
  keep, with N frames, a subset of what they'd keep with N+1 frames, at
  every point in the reference string. This guarantees monotonicity: more
  frames can never increase their fault count. That's a structural
  guarantee FIFO simply doesn't have.

## Thrashing

- **Thrashing**: a system state where processes spend more time
  page-faulting (swapping pages in/out) than doing actual useful work -
  each process has too few resident frames to hold its actively-used pages,
  so it constantly faults, which (under CPU-utilization-based schedulers)
  looks like low CPU usage, which the OS may misread as "not enough
  multiprogramming" and respond to by admitting *more* processes - shrinking
  everyone's frame share further and faulting even more. This vicious
  feedback loop is the defining trait of thrashing: the fix (fewer active
  processes) is the opposite of the naive symptom-driven response (add
  more).
- **Working-set model**: an admission-control strategy to prevent thrashing
  by tracking each process's *working set* - the set of pages it has
  referenced in the last Delta time units (a sliding window). A process is
  only allowed to run if enough frames are available to hold its entire
  current working set; if total system-wide working-set demand exceeds
  available frames, the OS suspends (swaps out entirely) one or more
  processes rather than let everyone starve for frames simultaneously. This
  directly targets the thrashing cause (too many processes' working sets
  competing for too few frames) instead of reacting to its CPU-utilization
  symptom.
