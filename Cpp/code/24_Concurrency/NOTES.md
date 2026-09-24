# Concurrency (C++ standard library)

- `std::thread`: joinable until `.join()` or `.detach()` is called; destroying a still
  joinable thread calls `std::terminate`. Prefer join unless you truly want fire-and-forget.
- `std::mutex` + `std::lock_guard`: RAII lock, unlocks on scope exit even if an exception
  is thrown. `std::unique_lock` is heavier but supports deferred locking, manual
  unlock/relock, and is required by `std::condition_variable::wait`.
- Deadlock avoidance: always acquire multiple mutexes in a fixed global order, or use
  `std::lock`/`std::scoped_lock` (C++17) to acquire several locks atomically.
- `std::condition_variable`: always wait with a predicate
  (`cv.wait(lock, []{ return ready; })`) to guard against spurious wakeups and missed
  notifications; the predicate re-checks the condition even if notified early/late.
- `std::atomic<T>`: lock-free (for suitably small/trivial T) read-modify-write ops.
  C++'s version is a typed template with member functions (`load`, `store`, `fetch_add`);
  C's `<stdatomic.h>` uses `_Atomic`-qualified types and free functions but shares the
  same `memory_order` values (relaxed, acquire, release, acq_rel, seq_cst).
  `memory_order_seq_cst` is the safe default; `relaxed` is fine when only the final value
  matters, not ordering relative to other memory.
- `std::future`/`std::async`/`std::promise`: `std::async(std::launch::async, ...)` runs on
  a new thread and returns a future whose `.get()` blocks until ready (and rethrows any
  exception the callable threw). `std::promise`/`std::future` is the manual, lower-level
  version of the same producer/consumer hand-off. `.get()` can only be called once per
  future.
- Thread pool: fixed worker threads pull `std::function<void()>` tasks off a
  mutex/condition_variable-guarded queue; avoids the cost of spawning a thread per task.
- Interview gotcha: data races on non-atomic shared state are undefined behavior even if
  "it usually works" — always protect with a mutex or use an atomic type.
- `std::shared_mutex` + `std::shared_lock`/`std::unique_lock` (C++17): a reader/writer
  lock. Multiple readers can hold a `shared_lock` at once; a writer takes a `unique_lock`
  and gets fully exclusive access. Worth it only when reads vastly outnumber writes —
  under heavy contention it can be slower than a plain `std::mutex`.
- `std::recursive_mutex`: lets the same thread `lock()` it multiple times (matched by an
  equal number of `unlock()`s); a plain `std::mutex` would deadlock the thread on the
  second nested lock. Gotcha: needing this is usually a code smell — it often means the
  locked/recursive logic should be split into a locked public entry point plus an
  unlocked private helper, rather than reaching for `std::recursive_mutex`.
- `std::call_once` + `std::once_flag`: guarantees an initializer runs exactly once even
  under a race from many threads, with no hand-rolled double-checked locking. Modern
  replacement for manual "check flag, lock, check again" singleton init. Note: a
  function-local `static` (as in `DesignPatterns/Creational/Singleton.cpp`'s
  `Logger::instance()`) already gets one-time thread-safe initialization for free via
  C++11 magic statics, so it doesn't need `call_once`; reach for `call_once` when the
  one-time setup isn't just "construct a single local static object".
- Deadlock demo (`10_deadlockAndScopedLock.cpp`): two threads locking two mutexes in
  opposite order can each block forever holding one mutex while waiting on the other
  (circular wait). Fix: `std::scoped_lock` (C++17) locks multiple mutexes atomically
  with an internal deadlock-avoidance algorithm, so acquisition order across threads no
  longer matters; `std::lock()` plus deferred `unique_lock`s is the pre-C++17 equivalent.

- Memory ordering levels (`07_memoryOrderingLevels.cpp`):
  - `memory_order_relaxed`: guarantees atomicity only (no torn reads/writes and a
    consistent modification order for that one atomic), with zero ordering guarantee
    relative to other memory operations. Fine for counters where only the final total
    matters. Unsafe as a "ready flag" publishing pattern: the store/load being atomic
    says nothing about whether a plain write before it is visible to whoever observes
    the flag.
  - `memory_order_acquire` / `memory_order_release`: the safe, cheaper publishing
    pattern. A release store synchronizes-with a matching acquire load that observes
    it, establishing happens-before: everything the writer did before the release
    store is guaranteed visible to the reader after the acquire load. Only orders
    around that one atomic, not a global order across unrelated atomics.
  - `memory_order_seq_cst`: the default when no order is given. Adds a single global
    total order agreed on by all threads across every seq_cst operation on every
    atomic, not just pairs of matching release/acquire. Strongest guarantee, but the
    most expensive, especially on weakly-ordered hardware (ARM) where it needs full
    memory barriers; relaxed and acquire/release avoid that cost.
  - Interview gotcha: a relaxed-store "ready flag" bug is often invisible on x86
    (strong memory model keeps stores in program order in practice) but can
    genuinely reorder and misbehave on ARM -- always match the ordering to the
    actual guarantee needed, don't rely on "it worked when I tested it".

## C++20 thread rendezvous primitives (not available here)

`std::barrier` and `std::latch` (C++20) provide reusable/one-shot thread
rendezvous and countdown synchronization, but require a newer standard library than
this toolchain ships — see `30_Cpp20Features/NOTES.md`'s environment-limitation note
for why C++20 library features aren't available here. No code for them is included in
this folder.
