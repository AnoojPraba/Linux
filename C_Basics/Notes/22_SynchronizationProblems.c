Three named "classic" concurrency problems, each a slightly harder variant
of the mutex/condition-variable/deadlock ideas already covered in
`49_Threads/`.

Bounded-buffer producer/consumer:

*   Extends the single-slot handoff in `49_Threads/03_conditionVariable.c`
    to a buffer that holds several items at once. The producer only blocks
    (on a "not full" condition) when every slot is occupied; the consumer
    only blocks (on a "not empty" condition) when the buffer has nothing in
    it - so several items can be in flight without either side waiting on
    every single item's handoff. See
    `code/50_SyncProblems/01_boundedBufferProducerConsumer.c`.

Reader-writer problem:

*   Plain mutual exclusion (one thread in the critical section at a time,
    as in `49_Threads/02_mutex.c`) is stricter than necessary when most
    access is read-only: many readers can safely overlap, since none of
    them modify anything. A writer still needs full exclusive access
    (no readers, no other writers) at the same time. The trick is
    tracking how many readers are currently active, and only the *first*
    reader in / *last* reader out actually touches the writer-exclusion
    lock. See `code/50_SyncProblems/02_readerWriter.c`.

Dining philosophers:

*   The textbook illustration of circular-wait deadlock: five philosophers,
    five forks, each philosopher needs both their left and right fork to
    eat. If everyone grabs their left fork first and then waits for their
    right, all five can end up each holding one fork and waiting on a
    neighbor holding the other - the same shape as the two-thread AB/BA
    deadlock in `49_Threads/05_deadlockAvoidance.c`, just with five
    participants forming a full circle instead of two forming a pair.
*   The fix is the same idea as that file's: break the circular ordering.
    Here, the highest-numbered philosopher picks up their forks in the
    opposite order from everyone else, so the cycle of "everyone is
    waiting on their neighbor" can never fully close. See
    `code/50_SyncProblems/03_diningPhilosophers.c`.
