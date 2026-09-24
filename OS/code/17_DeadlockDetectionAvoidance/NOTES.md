# Deadlock Detection and Avoidance

- Distinct from `14_SyncProblems` (producer/consumer, reader/writer, dining
  philosophers - races/starvation-focused). This folder is specifically about
  deadlock: threads permanently blocked waiting on each other.
- **Coffman conditions** - all four must hold simultaneously for deadlock:
  1. Mutual exclusion - a resource can be held by only one thread at a time.
  2. Hold-and-wait - a thread holds one resource while waiting for another.
  3. No preemption - a resource can't be forcibly taken from its holder.
  4. Circular wait - a cycle of threads each waiting on the next.
  Breaking any single condition prevents deadlock.
- **`01_deadlockDemo.c`**: two threads lock `mutexA`/`mutexB` in opposite
  order -> circular wait -> deadlock (the program hangs).
- **`02_deadlockFixed.c`**: fix via consistent lock ordering (always acquire
  `mutexA` before `mutexB`) - breaks circular wait, condition 4.
- **Other prevention/avoidance strategies**:
  - `pthread_mutex_trylock()` + backoff: try the second lock, and if it
    fails, release everything held so far and retry later - avoids
    hold-and-wait becoming permanent.
  - Lock hierarchies/ordering (as demoed): assign a global order to all
    locks and always acquire in that order.
  - Timeouts (`pthread_mutex_timedlock()`): give up and retry instead of
    waiting forever.
  - Reduce lock scope/granularity so fewer resources are held simultaneously.
- **Banker's algorithm** (conceptual, OS-course classic): an avoidance
  algorithm that only grants a resource request if the resulting state is
  still "safe" - i.e. there exists some order in which all processes could
  still finish given their declared maximum future needs. Requires knowing
  each process's maximum resource claim in advance, which is impractical in
  most real systems - taught more for the CS-theory background than as a
  real-world implementation technique.
- **Detection** (as opposed to avoidance): build a resource-allocation graph
  or wait-for graph and periodically check for cycles; if found, one holder
  is chosen as a victim to preempt/kill to break the cycle. Databases do this
  routinely (deadlock detection + transaction rollback).
