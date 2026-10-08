# Memory Model Litmus Tests, ABA and Safe Reclamation

## What a memory model gives you
- Compilers AND CPUs reorder memory operations. The C11 model defines which
  reorderings are visible to other threads, per `memory_order`:
  - `relaxed`: atomicity only, no ordering (counters, stats).
  - `release` (store) / `acquire` (load): if the acquire load reads the value
    written by the release store, everything before the store is visible after
    the load. Message passing, lock hand-off, publication of an initialized object.
  - `acq_rel`: both, for read-modify-write (CAS in a lock-free queue).
  - `seq_cst` (default): all seq_cst operations appear in ONE global order.
    Needed when different threads must agree on order of independent writes
    (Dekker/Peterson, the SB pattern).
- **x86-64 = TSO:** only store->load reorders (store buffer). acquire/release
  are free (plain `mov`); seq_cst stores need `xchg`/`mfence`.
- **ARMv8 / POWER / RISC-V weak:** loads and stores reorder broadly; release =
  `stlr`, acquire = `ldar`, seq_cst = `stlr`+`ldar` (with ordering between).
  Code that "works on x86" can break on ARM - a classic migration bug.
- A data race on a non-atomic is UB in C11 regardless of hardware.
- `volatile` is NOT atomic and gives no inter-thread ordering.

## The two litmus tests (`01_litmusMpSb.c`)
- **MP (message passing):** `data=1; flag=1` / `r1=flag; r2=data`. Seeing
  `flag==1, data==0` is forbidden with release/acquire, allowed with relaxed.
- **SB (store buffering):** `x=1; r1=y` / `y=1; r2=x`. `r1==r2==0` is allowed
  even with release/acquire (and on x86 hardware!), forbidden only with
  seq_cst. Why Dekker/Peterson mutual exclusion needs seq_cst or a fence.
- Other names to recognize: LB (load buffering), IRIW (independent reads of
  independent writes - tests multi-copy atomicity), 2+2W, coherence tests (CoRR).
- **Measured on this Pi:** every row counted 0 in 1M runs (see the RESULT note
  in the source), so the demo does not display the weak behavior here. Treat
  it as the harness pattern (coordinator release, per-iteration cache lines,
  cross-core misses) and use `litmus7`/`herd7` or other hardware to see
  reorderings. Be ready to say: "absence of a failure in testing doesn't prove
  correctness - I reason from the model and check with a model checker/TSan."

## ABA (`02_abaTaggedStack.c`)
- A CAS only compares the VALUE. If head goes A -> B -> A between your read and
  your CAS, the CAS succeeds although the structure changed; in a stack pop it
  installs a stale `next` pointing at a freed/reused node.
- Fixes: (1) **version tag** packed with the pointer/index (needs double-width
  CAS - `cmpxchg16b`/`casp` - or packing indices into 64 bits as in the demo);
  (2) **safe memory reclamation** so a node cannot be freed/reused while any
  thread may still hold it; (3) LL/SC hardware (ARM `ldxr/stxr`) detects any
  intervening write, not just value change, but is not portable C.

## Safe memory reclamation
- **Hazard pointers:** each reader publishes the pointer it is about to use; a
  deleter scans all hazard slots and defers freeing protected nodes. Bounded
  garbage, readers pay a store + fence per protected pointer.
- **Epoch-based reclamation (EBR):** threads announce entry to a global epoch;
  a node retired in epoch E is freed after all threads have advanced past E.
  Cheap reads, but one stalled thread blocks reclamation.
- **RCU:** readers do nothing (kernel: just preempt-disable); writers copy,
  publish with a release store, and wait for a grace period before freeing.
- **Reference counting** (atomic) is simple but adds contention on the count and
  has its own ABA/race at the "load pointer then increment" step (needs
  split/double-width counts or `atomic<shared_ptr>`).
- **Never free; pool + tag** (as in the demo) sidesteps reclamation for
  fixed-size node pools.

## Senior interviewer Q&A
**Q: Is a lock-free queue faster than a mutex-protected one?**
A: Not automatically. Lock-free guarantees system-wide progress (no thread's
stall blocks others), not speed. Under low contention a mutex is competitive;
CAS loops under heavy contention waste work and bounce cache lines. Choose
lock-free for latency-critical paths where preemption while holding a lock is
unacceptable (real-time, signal handlers, producers that must not block).

**Q: lock-free vs wait-free vs obstruction-free?**
A: Wait-free: every thread finishes in bounded steps. Lock-free: at least one
thread makes progress in a finite number of steps (others may starve).
Obstruction-free: a thread makes progress if it runs alone. A spinlock-based
structure is none of these.

**Q: Explain acquire/release with a real example.**
A: Producer fills a struct then `store_release(&ready, 1)`; consumer
`load_acquire(&ready)` then reads the struct. The release store cannot be
reordered after prior writes, the acquire load cannot be reordered before later
reads, and the synchronizes-with edge makes the struct writes visible.
Relaxed would allow the consumer to see `ready==1` with a stale struct.

**Q: Why is `seq_cst` the default and when do you weaken it?**
A: It's the easiest to reason about. Weaken to acquire/release/relaxed on hot
paths after proving correctness (and measuring - on x86 seq_cst loads are free,
the cost is in stores; on ARM both loads and stores cost more).

**Q: What is ABA and how do you prevent it?**
A: See above; tagged pointers, hazard pointers/EBR/RCU, or avoiding node
reuse. Mention that on 64-bit you can use the unused top 16 bits of a pointer
or index+tag packing, and that `cmpxchg16b` requires 16-byte alignment.

**Q: Is double-checked locking safe?**
A: Only with an atomic flag/pointer using release on publish and acquire on the
fast-path read (or `call_once`). A plain pointer check races and can expose a
partially constructed object.

**Q: How do you test concurrent code?**
A: ThreadSanitizer (data races, not all ordering bugs), stress tests with many
threads and randomized yields, model checkers (CDSChecker, `herd7`, Loom for
Rust), formal reasoning about invariants/linearization points, running on weak
hardware (ARM), and `-fsanitize=thread` in CI.
