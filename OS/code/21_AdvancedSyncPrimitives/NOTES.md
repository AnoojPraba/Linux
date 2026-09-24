# Advanced Synchronization Primitives

- Complements `09_Threads` (mutex/condvar/counting-semaphore basics) and
  `14_SyncProblems` (producer-consumer/reader-writer/dining-philosophers as
  *problems* solved with those basics) - this folder covers two primitives
  not demonstrated anywhere else in this repo: `pthread_rwlock_t` and
  `pthread_barrier_t`.

## `pthread_rwlock_t` (`01_rwlockDemo.c`)

- Lets any number of readers hold the lock concurrently, but a writer needs
  exclusive access (blocks/is blocked by everyone).
- vs plain `pthread_mutex_t`: a mutex serializes *all* access, readers
  included - a rwlock only serializes writers, so read-heavy workloads with
  rare writes get much better concurrency.
- Tradeoff: reader starvation risk. If readers keep arriving faster than they
  drain, a waiting writer can be starved indefinitely on implementations that
  favor readers. glibc's rwlock defaults to a "prefer writer" policy to
  mitigate this, but exact behavior is implementation-defined - a plain mutex
  is starvation-fair (FIFO-ish) by comparison, at the cost of no read
  concurrency.
- Note `14_SyncProblems/02_readerWriter.c` solves the same conceptual problem
  by hand with a mutex + condition variable (to allow custom starvation
  policy, e.g. writer-priority) - `pthread_rwlock_t` is the "just use the
  library primitive" version when you don't need that custom control.

## `pthread_barrier_t` (`02_barrierDemo.c`)

- A rendezvous point: N threads call `pthread_barrier_wait()`, and none of
  them return from that call until all N have arrived. Then all are released
  together.
- Use case: phased parallel algorithms (e.g. iterative simulations, parallel
  matrix operations) where every thread must finish phase K before any
  thread starts phase K+1 - unlike a condition variable (which signals one
  specific event), a barrier is reusable per-phase and requires no manual
  counting/predicate logic.
- One arbitrary caller (unspecified which) gets `PTHREAD_BARRIER_SERIAL_THREAD`
  as its return value instead of 0 - handy for electing one thread to do
  single-threaded setup/cleanup between phases without extra coordination.
- Contrast with `pthread_join()`: join waits for full thread termination
  (one-shot); a barrier is a mid-execution sync point threads pass through
  repeatedly and keep running afterward.
- Not the same as a mutex/semaphore: those grant *exclusive or limited*
  access; a barrier grants *no* access, it just holds everyone back until
  the whole group is ready.

## From-scratch fair rwlock (`03_fairRwLockFromScratch.c`)

- Built from just a `pthread_mutex_t` and two condition variables, tracking
  active-reader count, active-writer count, and waiting-writer count by
  hand.
- Explicitly fair: a new reader blocks if there is an active *or waiting*
  writer, not just an active one - checking `waitingWriters` (not only
  `activeWriters`) is precisely what stops a constant stream of readers from
  starving a writer that's already queued.
- Contrast with `pthread_rwlock_t` (`01_rwlockDemo.c`): the library
  primitive's fairness policy is implementation-defined, and on glibc/Linux
  it favors readers by default - a writer can be starved under sustained
  read load unless the lock is created with
  `PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP`. The from-scratch version
  bakes writer-starvation prevention into the acquire logic itself, with no
  attribute flag to remember.
- The value of building it by hand isn't to replace `pthread_rwlock_t` in
  real code (the library version is well-tested and usually the right
  choice) - it's that understanding the counts/predicates a fair rwlock
  needs makes it possible to reason correctly about *any* rwlock's
  starvation behavior, including tuning or auditing the library one.
