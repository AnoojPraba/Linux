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
