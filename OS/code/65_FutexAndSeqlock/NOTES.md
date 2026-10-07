# Futex and Seqlock

## Futex (fast userspace mutex)
- `futex(2)` is the single kernel primitive under pthread mutexes, condvars,
  semaphores, rwlocks and `call_once`. Idea: keep lock state in a user-space
  integer; only enter the kernel to **sleep** (`FUTEX_WAIT`) or **wake**
  (`FUTEX_WAKE`) when there is contention.
- Uncontended lock/unlock = one atomic instruction, no syscall (~ns). A
  contended one = syscall + context switch (~us).
- `FUTEX_WAIT(addr, val)`: kernel atomically checks `*addr == val` and sleeps
  - closes the lost-wakeup race between "I saw it locked" and "I went to
  sleep". Waiters are queued in a kernel hash table keyed by the address
  (physical for shared mappings, virtual for `_PRIVATE`).
- `_PRIVATE` flag: process-local, avoids the cross-process lookup - faster.
- Three-state mutex (0 unlocked / 1 locked / 2 locked+waiters) avoids the
  wake syscall on unlock when nobody waits. Naive 2-state needs a syscall on
  every unlock. See `01_futex_mutex.c`.
- Other ops: `FUTEX_REQUEUE`/`CMP_REQUEUE` (condvar broadcast without
  thundering herd), `FUTEX_LOCK_PI` (priority-inheritance mutexes, fixes the
  inversion in `../16_PriorityInversion`), `FUTEX_WAIT_BITSET` (absolute
  timeouts, used by `pthread_mutex_timedlock`), robust futexes (kernel
  releases locks of a dead owner -> `EOWNERDEAD`).
- Hybrid spin-then-sleep (adaptive mutex, `PTHREAD_MUTEX_ADAPTIVE_NP`): spin
  briefly if the owner is running on another core, then futex-wait.
- Not available to portable code - use pthreads; know it as the mechanism.
  macOS has `os_unfair_lock`/`__ulock_wait`, Windows `WaitOnAddress`; C++20
  `std::atomic::wait/notify` maps onto futex.
- Debug: `strace -f -e futex` shows contention as a stream of futex calls; a
  program with no `futex` calls is running its lock fast path only.

## Seqlock
- Writer bumps a sequence counter before and after the update (odd =
  in-progress). Readers copy data and retry if the counter was odd or changed.
  Readers never write shared memory -> no cache-line bouncing and readers
  cannot starve the writer (unlike a reader-writer lock).
- Trade-offs: writer may be starved by none, but READERS can starve under a
  hot writer (retry loop); payload must be plain data (no pointers followed
  mid-read); multiple writers still need a lock among themselves.
- Memory ordering is the subtle part: writer needs a release fence between
  the first counter store and the data stores (and release on the final
  store); reader needs acquire loads/fence so data reads are ordered before the
  second counter read. In strict C11 the data fields must be atomics (relaxed)
  to avoid a formal data race; the Linux kernel uses `READ_ONCE`/`WRITE_ONCE`
  plus barriers.
- Compare: spinlock (all exclude each other), rwlock (readers write the lock
  word), RCU (readers fully lock-free, writers copy-and-publish; see
  `../38_ConcurrentDataStructures`), seqlock (cheap readers on small data).
- Interview framing: "I'd use a seqlock for a timestamp/config snapshot read
  millions of times per second and updated occasionally, since readers never
  contend and the data is a few words."
- Related: `../20_Atomics`, `../21_AdvancedSyncPrimitives`, `../40_CacheCoherenceMESI`.

## Senior interviewer Q&A
**Q: How is a pthread mutex implemented on Linux?**
A: A user-space atomic word plus futex. Lock: CAS 0->1; on contention set the
state to "locked with waiters" and `FUTEX_WAIT`. Unlock: if state indicates
waiters, store 0 and `FUTEX_WAKE` one thread. Uncontended paths make no
syscall. (`01_futex_mutex.c` is this algorithm.)

**Q: Why does `FUTEX_WAIT` take the expected value?**
A: The kernel atomically compares `*addr` to the value and only then sleeps.
That closes the race where the lock is released between the user-space check
and the sleep - otherwise the wakeup is lost and the thread sleeps forever.

**Q: Why three states instead of two?**
A: With two (locked/unlocked), unlock cannot tell whether anyone sleeps, so it
must always issue `FUTEX_WAKE` - a syscall on every unlock even when
uncontended. The third state ("waiters") lets unlock skip the syscall when
nobody waits.

**Q: How would you diagnose lock contention in a production C service?**
A: `strace -f -c -e futex` or `perf trace` to see futex volume; `perf record`
+ flame graphs for time in `__lll_lock_wait`; lock profilers (`perf lock`,
eBPF `offcputime`, mutrace). A mutex that never shows futex calls is
uncontended. Fixes: shrink critical sections, shard locks, per-thread data,
reader-writer/RCU/seqlock for read-heavy, or redesign.

**Q: Mutex vs spinlock - when each?**
A: Spinlock: critical section shorter than a context switch, lock holder
never sleeps/gets preempted (kernel with preemption disabled, pinned
threads). In user space, pure spinlocks risk burning a time slice when the
holder is descheduled; adaptive mutexes spin briefly then futex-wait.

**Q: What is priority inversion and how does futex help?**
A: A low-priority holder blocks a high-priority waiter while a medium-priority
thread preempts the holder. `PTHREAD_PRIO_INHERIT` mutexes use `FUTEX_LOCK_PI`
so the kernel boosts the holder to the waiter's priority. See
`../16_PriorityInversion`.

**Q: What happens if a thread dies holding a mutex?**
A: A normal mutex stays locked forever (deadlock). Robust mutexes
(`PTHREAD_MUTEX_ROBUST`) use the kernel's robust-futex list: the next locker
gets `EOWNERDEAD`, must repair state and call `pthread_mutex_consistent`.
Critical for process-shared mutexes in shared memory.

**Q: Explain a seqlock and when you would use it over a reader-writer lock.**
A: Writer increments a counter (odd while writing); readers snapshot, then
retry if the counter was odd or changed. Readers never write shared memory, so
there is no cache-line bouncing and they never block writers. Use for small,
read-hot, write-rare data (time, config snapshots, statistics). Not for
data with pointers followed during the read, nor many writers.

**Q: What are the memory-ordering requirements of a seqlock?**
A: Writer: counter store (odd) before data stores; data stores before the
final counter store (release). Reader: counter load (acquire), then data
reads, then a fence, then re-read the counter. In C11 the data words must be
atomic (relaxed) to avoid a formal data race even though torn results are
discarded.
*Follow-up: why can readers starve?* A continuously writing producer makes
every read attempt overlap a write (demo: millions of retries). Add rate
limiting or fall back to a lock after N retries.

**Q: Compare RCU, seqlock and rwlock.**
A: rwlock: readers take a shared lock (write the lock word -> cache traffic),
writers exclude all. Seqlock: readers lock-free but may retry; copies data;
single-writer-friendly. RCU: readers lock-free, never retry, dereference a
pointer to a stable snapshot; writers copy-update then wait a grace period
before freeing - best for read-mostly pointer-based structures.
