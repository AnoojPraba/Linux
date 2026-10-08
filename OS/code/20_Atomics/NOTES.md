# Atomics (C11 `<stdatomic.h>`)

Demos: `01_stdatomicCounter.c` (atomic counter vs mutex), `02_threadLocalStorage.c`
(per-thread state). Memory ordering, litmus tests and ABA: `../66_MemoryModelLitmusTests`;
futex/seqlock: `../65_FutexAndSeqlock`; lock-free queues: `../37_LockFreeRingBuffer`.

## What an atomic gives you
- **Atomicity:** read-modify-write happens as one indivisible step; no torn values.
- **Ordering:** each operation takes a `memory_order` controlling what other
  threads may observe around it (relaxed/acquire/release/acq_rel/seq_cst).
- Operations: `atomic_load/store`, `atomic_exchange`, `atomic_fetch_add/sub/and/or/xor`,
  `atomic_compare_exchange_strong/weak`, `atomic_thread_fence`, `atomic_flag`
  (the only type guaranteed lock-free), `atomic_is_lock_free`.
- Hardware: x86 `lock xadd`/`cmpxchg`; ARMv8.1 LSE `ldadd`/`cas`, or older
  LL/SC (`ldxr/stxr`) loops - so `compare_exchange_WEAK` may fail spuriously (use
  in loops; `strong` for single attempts).

## Atomics vs `volatile` vs mutex
| | atomic | volatile | mutex |
|---|---|---|---|
| Atomic RMW | yes | **no** | yes (via critical section) |
| Inter-thread ordering | per memory_order | none | acquire on lock, release on unlock |
| Prevents compiler optimizing away/reordering access | yes (per order) | prevents elision only | yes |
| Use for | counters, flags, lock-free structures | MMIO registers, signal flags (`sig_atomic_t`), `setjmp` | compound invariants over several variables |
`volatile` is for memory the hardware or a signal handler changes; it is NOT a
threading primitive (`../11_VolatileVsAtomicEmbedded`).

## Patterns
- **Counter/statistics:** `fetch_add(..., relaxed)` - ordering not needed, only atomicity.
- **Flag/publish:** producer `store_release(&ready, 1)` after writing data; consumer
  `load_acquire(&ready)` then reads data.
- **Spinlock:** `while (atomic_exchange_explicit(&l, 1, memory_order_acquire)) pause;`
  unlock `store_release(&l, 0)`; add test-and-test-and-set + backoff to avoid cache
  line hammering.
- **CAS loop:** `old = load; do { new = f(old); } while (!cas_weak(&x, &old, new));`
- **Reference count:** `fetch_add(relaxed)` to increment, `fetch_sub(release)` and an
  acquire fence on the thread that sees the count hit zero before freeing.
- **Once-init:** `atomic` flag + acquire/release, or `pthread_once`.

## Pitfalls
- Two atomics do not make an atomic pair (check-then-act races): e.g. `if (a == 0) a = 1`
  is a race; use `compare_exchange`.
- Atomic != lock-free: large types (`_Atomic struct`) may use hidden locks;
  check `atomic_is_lock_free`.
- ABA with pointer CAS; reclamation of nodes (`../66_MemoryModelLitmusTests`).
- False sharing between adjacent atomics used by different threads (`../39_FalseSharing`).
- Mixing atomic and non-atomic access to the same object is a data race (UB).
- `seq_cst` default is safe but costs a full barrier on stores on ARM/POWER;
  relaxed counters are the cheapest.
- Signal handlers: only lock-free atomics (or `sig_atomic_t`) are async-signal-safe.

## Thread-local storage (`02_threadLocalStorage.c`)
`_Thread_local` removes sharing entirely: per-thread counters aggregated at the
end beat contended atomics (`../../../C_Basics/code/80_SignalSafetyAndThreadLocal`).

## Senior interviewer Q&A
**Q: Why is `counter++` unsafe across threads even on a single-core machine?**
A: It's load-add-store; a context switch between load and store lets another
thread's update be overwritten (lost update), and the compiler may also keep it in a
register. Use `atomic_fetch_add` or a lock. Single-core only removes hardware
reordering, not preemption races.

**Q: What does `memory_order_relaxed` guarantee?**
A: Atomicity and a single modification order per variable, nothing about ordering
relative to other variables. Perfect for counters, wrong for publishing data.

**Q: Explain release/acquire in one example.**
A: Writer: `data = 42; store_release(&flag, 1);` Reader: `while (!load_acquire(&flag));
use(data);`. Reading `flag == 1` through the acquire load synchronizes-with the release
store, so `data == 42` is guaranteed visible.

**Q: When do you need `seq_cst`?**
A: When threads must agree on a single global order of operations to independent
variables, e.g. Dekker/Peterson locks and the store-buffering pattern; otherwise
acquire/release suffices.

**Q: `compare_exchange_weak` vs `strong`?**
A: Weak may fail spuriously (LL/SC architectures) but is cheaper in loops; strong
retries internally. Use weak inside CAS loops, strong when a failure has meaning
and you aren't looping.

**Q: Is an atomic counter always better than a mutex?**
A: For a single word, yes mostly - no blocking, no syscalls. But under heavy
contention, even atomics serialize on one cache line; per-thread/sharded counters
(aggregated on read) scale better.

**Q: How would you implement a spinlock correctly?**
A: Acquire on lock, release on unlock, test-and-test-and-set, a CPU `pause`/`yield`
hint, backoff or fall back to a futex-based mutex; never use spinlocks in user space
where the holder can be descheduled unless pinned/short.

**Q: How do atomics relate to the kernel's `READ_ONCE/WRITE_ONCE`?**
A: Those force a single non-torn, non-elided access (like relaxed atomics) for
shared data in kernel C that predates C11; barriers like `smp_mb()`/
`smp_load_acquire()` provide ordering.
