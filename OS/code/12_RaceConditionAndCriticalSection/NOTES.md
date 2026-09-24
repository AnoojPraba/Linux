# Race Conditions and Critical Sections

- **Race condition**: a bug class where the outcome of a program depends on
  the relative timing/interleaving of concurrent accesses to shared state.
  Same code, same inputs, different run -> different (wrong) result, because
  the OS scheduler can interleave thread execution differently each time.
- **Critical section**: the region of code that accesses shared state and
  therefore must run with mutual exclusion - only one thread may execute
  its critical section for a given shared resource at a time.
- **`01_unprotectedCounterRace.c`**: `counter++` looks atomic in source but
  is really three steps - load `counter` into a register, add one, store it
  back. If thread A's load happens before thread B's store lands, A's
  increment gets overwritten by B's - a lost update. Running the demo
  typically prints a final count noticeably below the expected total.
- **Three requirements for a correct critical-section solution** (the
  classic OS-course formulation):
  1. **Mutual exclusion** - at most one thread may be inside the critical
     section for a given resource at any time.
  2. **Progress** - if no thread is in the critical section and some
     threads want to enter, only those threads (not ones outside,
     uninvolved) get to decide who enters next, and that decision can't be
     postponed forever - the system doesn't stall with the resource idle
     while there's outstanding demand.
  3. **Bounded waiting** - there's a bound on how many times other threads
     may enter the critical section after a given thread has requested
     entry and before that request is granted - no thread waits forever
     while others repeatedly cut in line (no starvation).
- Fixing the demo means wrapping `counter++` in a critical section (e.g. a
  `pthread_mutex_t`, as in `09_Threads/02_mutex.c`, or an `atomic_int`, as in
  `20_Atomics/01_stdatomicCounter.c`) so the read-modify-write is
  indivisible with respect to other threads.
