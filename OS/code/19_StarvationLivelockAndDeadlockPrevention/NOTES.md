# Starvation, Livelock, and Deadlock Prevention

NOTES.md-only - a conceptual companion to `17_DeadlockDetectionAvoidance`
(deadlock detection/avoidance mechanics) and
`18_ResourceAllocationGraphAndBankersAlgorithm` (the RAG/Banker's
formalization), rounding out the deadlock-adjacent liveness topics with
prevention strategies and the starvation/livelock distinction. See also the
scheduling concepts folder for aging as a general starvation fix in the
scheduler context.

## Deadlock prevention

Unlike avoidance (Banker's algorithm - grant requests only if the resulting
state stays safe) or detection (periodically scan for cycles and pick a
victim), **prevention** structurally rules out deadlock by attacking one of
the four Coffman conditions so it can never hold:

1. **Mutual exclusion** - hardest to eliminate in general (some resources
   are inherently exclusive, e.g. a write lock), but where possible, make
   the resource shareable instead (e.g. read-only/reader-writer access,
   or spooling a printer instead of granting exclusive device access).
2. **Hold-and-wait** - eliminate by requiring a process to request *all* the
   resources it will ever need at once, up front, before it starts (or to
   release everything it holds before requesting more). Downside: poor
   resource utilization (resources sit reserved but unused for the
   process's whole lifetime) and requires knowing needs in advance.
3. **No preemption** - allow a resource to be forcibly taken from its
   holder (and given back later) if the holder can't get an additional
   resource it needs. Practical mainly for resources whose state can be
   saved/restored cleanly (e.g. CPU registers via a context switch); hard
   for resources like a printer mid-job or a held lock protecting
   in-progress mutation.
4. **Circular wait** - eliminate by imposing a total (global) ordering on
   all resource types and requiring every process to request resources
   only in increasing order. This is the fix demonstrated concretely in
   `17_DeadlockDetectionAvoidance/02_deadlockFixed.c` (always acquire
   `mutexA` before `mutexB`) - no cycle can form if everyone follows the
   same order.

## Starvation vs livelock

- **Starvation**: a thread/process is perpetually denied a resource it
  needs, even though the resource does periodically become available -
  other threads keep "cutting in line" ahead of it (e.g. a low-priority
  thread that never gets scheduled because higher-priority threads keep
  arriving, or a writer that never gets a reader-preferring rwlock because
  readers keep arriving). The starved thread is typically blocked/waiting.
  **Fix**: aging - gradually increase a waiting thread/process's effective
  priority the longer it waits, so it's eventually guaranteed to win
  (see the scheduling concepts folder for aging in scheduler algorithms;
  the Bakery algorithm in `13_ClassicalSyncAlgorithms/03_bakeryAlgorithm.c`
  achieves the analogous guarantee for lock acquisition via strict ticket
  ordering rather than priority aging).
- **Livelock**: threads are *not* blocked - they're actively running and
  changing state in direct response to each other, but the group makes no
  real progress. Classic real-world analogy: two people in a hallway both
  step aside to let the other pass, then both step back, repeatedly,
  neither getting through. In software: two threads that each detect
  potential contention and both back off/retry at the same interval,
  perpetually colliding again on retry (e.g. two `trylock()`-based deadlock
  -avoidance loops that release and retry in lockstep). **Fix**: add
  randomized/exponential backoff so retries desynchronize, or fall back to
  a fixed tie-breaking order (similar in spirit to a lock hierarchy) so one
  side always yields.
- Key distinction: starvation involves waiting (idle, denied a turn);
  livelock involves both parties actively doing work but never converging -
  CPU usage looks "alive" in livelock but nothing useful happens.
