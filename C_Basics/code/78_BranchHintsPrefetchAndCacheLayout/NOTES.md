# Branch Hints, Prefetch and Cache-Friendly Layout

- **Branch prediction:** CPUs speculate past branches; a mispredict flushes
  the pipeline (~10-20 cycles). Predictable branches (sorted data, loops) are
  nearly free; data-dependent random ones are expensive. Classic question:
  "why is this loop faster on sorted input?" - `01_likely_and_branches.c`.
- Fixes for unpredictable branches: branchless arithmetic/masks, conditional
  moves (compiler may do it itself), lookup tables, sorting/partitioning
  data first. Measure - at `-O2` the compiler often already emits `cmov`.
- **`__builtin_expect` / `likely` / `unlikely`:** tell the compiler which path
  is hot so it lays out the cold path out of line (better i-cache use). It is
  a layout hint, not a CPU predictor hint on most architectures. Use on
  genuinely skewed paths (error handling), not everywhere. C++20 has
  `[[likely]]`/`[[unlikely]]`. `__builtin_unreachable()` and
  `__attribute__((cold))` are related tools.
- **Software prefetch:** `__builtin_prefetch(addr, rw, locality)`. Only helps
  when the address is known early but the hardware prefetcher cannot predict
  it (indirect/gather access, hash probing, tree/list traversal with batched
  lookups). Prefetch distance must cover memory latency; too early evicts
  before use, too late does nothing. Harmless to a bad address (no fault).
  Sequential scans do not need it.
- **AoS vs SoA:** AoS keeps an object's fields together (good when you touch
  all fields); SoA keeps each field contiguous (good when loops touch one or
  two fields, and for SIMD). Hybrid AoSoA (blocks of 8/16) balances both.
  Cache line = 64 bytes on x86 and most ARM, so a scan of one 4-byte field in
  a 32-byte struct uses 1/8 of the bytes it loads.
- **Hot/cold splitting:** move rarely used fields out of the hot struct so
  more objects fit per cache line.
- Other layout rules: align hot data to cache lines (`_Alignas(64)`) but avoid
  false sharing between threads (`../../../OS/code/39_FalseSharing`); keep struct
  members ordered to minimize padding (`../59_MemoryAlignmentAndPadding`);
  prefer arrays/indices over pointer-chasing lists
  (`../36_AdvancedHashingAndCacheAwareStructures`).
- Inline assembly (when asked): GNU syntax
  `asm volatile("template" : outputs : inputs : clobbers)`. An empty template
  with a `"memory"` clobber is a compiler barrier (stops reordering, emits no
  instruction); it is NOT a CPU barrier - use `atomic_thread_fence` or
  `dmb`/`mfence` for that. Prefer intrinsics (`<arm_neon.h>`, `<immintrin.h>`)
  or builtins over hand-written asm.
- Always measure with `perf stat -e cache-misses,branch-misses` (see
  `../69_PerfAndStrace`); micro-benchmarks lie without warm-up, repetition and
  a defeated optimizer.

## Senior interviewer Q&A
**Q: Why is processing a sorted array faster than an unsorted one in the same loop?**
A: The data-dependent branch becomes predictable, avoiding pipeline flushes
(~10-20 cycles each). In the demo: ~0.31s random vs ~0.13s sorted on the
Pi. Sorting is not free, so it pays only when the sorted order is reused or
the pass count is large.
*Follow-up: how else can you remove the penalty?* Branchless masks, `cmov`,
lookup tables, SIMD compare + blend.

**Q: What does `__builtin_expect` actually do?**
A: Biases the compiler's basic-block layout (and sometimes inlining/register
decisions) so the hot path falls through and cold code is moved away. It does
not directly program the CPU predictor on most targets. Misuse (wrong hint)
costs a little; profile-guided optimization (`-fprofile-use`) usually beats
hand hints.

**Q: When does software prefetch help? When does it hurt?**
A: Helps for predictable-in-software but unpredictable-in-hardware access:
gather/scatter, hash probing, pointer-chasing with known next nodes, batched
lookups. Distance must match memory latency / loop work. Hurts when too early
(evicted before use), too late (no benefit), or redundant on sequential scans
(hardware prefetcher already works) - it adds instructions and pollutes
cache. Always measure.

**Q: AoS or SoA?**
A: Depends on access pattern. Loops that touch one or two fields over many
objects (physics, analytics, ECS) favor SoA: full cache-line utilization and
easy SIMD. Code that touches all fields of one object at a time (OO-style,
random access by id) favors AoS. AoSoA blocks give both.
*Follow-up: how would you prove it?* `perf stat -e cache-misses,instructions`
before/after, and compare bytes loaded to bytes used.

**Q: What is false sharing and how does it relate?**
A: Independent variables on one 64B line written by different cores bounce
the line between caches. Pad/align hot per-thread data to a cache line
(`_Alignas(64)`); the opposite concern of packing cold data tightly. See
`../../../OS/code/39_FalseSharing`.

**Q: What is a compiler barrier vs a memory barrier?**
A: `asm volatile("" ::: "memory")` stops the compiler reordering/caching
memory accesses across it but emits no instruction. A CPU barrier
(`dmb ish`, `mfence`, `atomic_thread_fence`) orders hardware-visible memory
operations. Neither replaces atomics for inter-thread communication.

**Q: Why might your micro-benchmark be lying?**
A: Dead-code elimination, loop hoisting, vectorization of one variant only,
cold caches / page faults on first touch, frequency scaling and noisy
neighbors, too few iterations, measuring wall clock with other load. Fix:
`volatile` sinks or `asm` clobbers, warm-up, repeats with min/median,
`perf stat`, pin the CPU.
