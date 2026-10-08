# 24_Concurrency

C++ standard-library concurrency from threads and mutexes to condition variables, atomics with memory orderings, futures, thread pool, reader/writer locks, call_once, deadlock and a lock-striped LRU cache.

## Files
- `01_threadBasics.cpp` - std::thread entry points, join
- `02_mutexAndLockGuard.cpp` - lock_guard, two-mutex ordering, std::lock/scoped_lock
- `03_conditionVariableProducerConsumer.cpp` - producer/consumer queue with condition_variable and predicate
- `04_atomics.cpp` - std::atomic with relaxed ordering
- `05_futureAsyncPromise.cpp` - std::async/future and promise
- `06_threadPool.cpp` - fixed worker pool over a mutex/cv-guarded task queue
- `07_memoryOrderingLevels.cpp` - relaxed vs acquire/release vs seq_cst
- `07_sharedMutexReaderWriter.cpp` - shared_mutex with shared_lock readers
- `08_recursiveMutex.cpp` - recursive_mutex and why it is a smell
- `08_threadSafeLruCacheLockStriped.cpp` - thread-safe LRU cache with lock striping (cf. C_Basics 37_LRUCache)
- `09_callOnce.cpp` - call_once/once_flag one-time init
- `10_deadlockAndScopedLock.cpp` - opposite-order locking deadlock; the deadlock-prone path is never called from main(); scoped_lock fix
- `NOTES.md` - one bullet per primitive plus gotchas; C++20 rendezvous primitives noted as unavailable here

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_threadBasics.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/24_Concurrency/` (git-ignored).

## Key concepts / interview angles
- A joinable `std::thread` destroyed without join/detach calls `std::terminate`.
- Always wait on a condition variable with a predicate (spurious wakeups, missed notifications).
- Data races are UB; use a mutex or `std::atomic`.
- Release/acquire publishes writes; relaxed only guarantees atomicity; seq_cst is the default.
- Deadlock avoidance: fixed lock order or `scoped_lock`.
- Reader/writer locks only pay off when reads dominate.

## Gotchas
- Thread output interleaving and timings are nondeterministic.
- Two files each share the `07_` and `08_` prefixes (existing naming quirk; keep names as they are).
- Check races with `g++ -std=c++17 -g -fsanitize=thread -pthread <file>.cpp`.

## Related
- `../../../OS/code/09_Threads`
- `../../../OS/code/20_Atomics`
- `../../../OS/code/37_LockFreeRingBuffer`
- `../../../C_Basics/code/37_LRUCache`
- `../DesignPatterns/Creational/Singleton.cpp`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
