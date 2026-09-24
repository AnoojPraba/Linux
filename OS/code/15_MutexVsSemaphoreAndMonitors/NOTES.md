# Mutex vs Semaphore, and Monitors

NOTES.md-only folder - no standalone demo, since the primitives discussed
are already demonstrated in `09_Threads` (mutex, condition variable,
counting semaphore) and `14_SyncProblems` (those primitives applied to
producer-consumer/reader-writer/dining-philosophers). This folder is the
conceptual comparison layer on top of those demos.

## Mutex vs semaphore

- **Ownership**: a mutex has a notion of ownership - the thread that locked
  it must be the one to unlock it (`pthread_mutex_unlock` from a different
  thread than the locker is undefined behavior). A semaphore has no
  ownership - any thread can call `sem_post()`, even one that never called
  `sem_wait()`.
- **Binary vs counting**: a mutex is inherently binary (locked/unlocked). A
  semaphore holds an integer count and can be binary (count capped at 1,
  behaving like a mutex without ownership) or counting (initialized to N,
  allowing up to N concurrent holders).
- **Use case split**: use a mutex to protect a critical section around
  shared data (see `09_Threads/02_mutex.c`). Use a counting semaphore to
  manage a pool of N interchangeable resources (e.g. N available buffer
  slots in `14_SyncProblems/01_boundedBufferProducerConsumer.c`, or N
  permitted concurrent connections) - the count directly models "how many
  are available right now," which a mutex has no way to express.
- A semaphore can also be used for signaling between threads (e.g. "wake up,
  an item is ready") in a way a mutex cannot, since a mutex must be
  unlocked by its own locker - see `09_Threads/04_semaphore.c` and
  `09_Threads/03_conditionVariable.c` (a condition variable plus a mutex is
  the more idiomatic way to do that signaling in C).

## Monitors

- A monitor is a higher-level construct that bundles shared data with the
  procedures that operate on it, plus built-in mutual exclusion so only one
  thread executes inside the monitor at a time (Java's `synchronized`
  methods/blocks; C++ classes wrapping a `std::mutex` and exposing only
  synchronized methods, e.g. `std::lock_guard` inside every member
  function).
- C has no language-level monitor construct - no keyword automatically wraps
  a function body in a lock. The closest analogue is a plain convention: a
  struct holding both the data and a `pthread_mutex_t`, with every function
  that touches the struct locking/unlocking that mutex internally so
  callers never manage the lock directly. A tiny illustrative sketch:

  ```c
  typedef struct
  {
      int value;
      pthread_mutex_t lock;
  } ThreadSafeCounter;

  void threadSafeCounterIncrement(ThreadSafeCounter *counter)
  {
      pthread_mutex_lock(&counter->lock);
      counter->value++;
      pthread_mutex_unlock(&counter->lock);
  }
  ```

  Callers never see or touch `counter->lock` directly - every access goes
  through a function that already enforces mutual exclusion, which is the
  same guarantee a monitor gives, just via convention (every accessor
  function does this) instead of language enforcement (the compiler forces
  it for every method).
- Monitors typically pair with condition variables internally for
  wait/notify (Java's implicit monitor `wait()`/`notify()`, C++'s
  `std::condition_variable` used alongside the wrapping class's mutex) - the
  same building block as `09_Threads/03_conditionVariable.c`, just packaged
  behind the class/object interface instead of used directly by callers.
