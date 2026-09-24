# Priority Inversion

- **Priority inversion**: a high-priority thread is blocked waiting on a
  lock held by a low-priority thread, and a medium-priority thread (which
  needs no lock at all) preempts the low-priority holder. The low-priority
  thread can't run to finish and release the lock, so the medium-priority
  thread indirectly starves the high-priority thread - a lower-priority
  thread effectively delays a higher-priority one, inverting the scheduler's
  intended priority order.
- **Mars Pathfinder (1997)**: the canonical real-world example. A low-
  priority meteorological data-gathering task held a mutex a high-priority
  bus-management task needed; a medium-priority communications task
  repeatedly preempted the low-priority task, so the high-priority task
  missed its deadlines for long enough that a watchdog timer fired and
  reset the whole spacecraft. Fixed remotely by enabling priority
  inheritance on the mutex.
- **`01_priorityInversionDemo.c`**: three pthreads (low/medium/high) using
  `pthread_setschedparam(SCHED_FIFO, ...)` to request real-time priorities,
  walking through the scenario: low acquires the shared lock, high blocks
  waiting on it, medium (no lock needed) runs concurrently. This is
  **illustrative only** - real priority inversion is a timing-sensitive
  scheduler phenomenon, and reliably reproducing the actual starvation
  requires root/`CAP_SYS_NICE` privileges (needed for `SCHED_FIFO`) and a
  system under enough load for preemption to matter; the demo documents the
  scenario via structure and comments rather than guaranteeing a measurable
  inversion on every run/machine.
- **Fix 1 - priority inheritance**: while a high-priority thread waits on a
  lock, temporarily boost the holding thread's priority to match (or
  exceed) the waiter's, so a medium-priority thread can no longer preempt
  it. The holder drops back to its original priority on release. Linux
  supports this via `PTHREAD_PRIO_INHERIT` passed to
  `pthread_mutexattr_setprotocol()`.
- **Fix 2 - priority ceiling protocol**: assign every lock a static priority
  ceiling equal to the highest priority of any thread that might ever lock
  it; a thread that acquires the lock immediately runs at that ceiling
  priority for the duration it holds it. This prevents inversion
  preemptively (no medium-priority thread can ever have higher priority
  than the ceiling) rather than reactively boosting on contention, at the
  cost of needing to know all potential lockers' priorities up front.
  Linux exposes this via `PTHREAD_PRIO_PROTECT` plus
  `pthread_mutexattr_setprioceiling()`.
- Contrast with `17_DeadlockDetectionAvoidance`: priority inversion doesn't
  require a cycle of waiting threads (no deadlock) - the low-priority
  holder is never blocked, just never scheduled. It's a liveness/fairness
  problem in the scheduler's interaction with locking, not a Coffman-
  conditions deadlock.
