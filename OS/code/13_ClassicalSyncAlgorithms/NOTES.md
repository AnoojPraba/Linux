# Classical Mutual Exclusion Algorithms

- Before CPUs offered atomic read-modify-write instructions (test-and-set,
  compare-and-swap), mutual exclusion between concurrent threads/processes
  had to be built entirely from ordinary loads and stores of shared
  variables. These "software-only" algorithms are a foundational part of OS
  theory even though real systems now use hardware primitives instead.
- **`01_petersonsAlgorithm.c`**: two-thread solution using a `flag[]` array
  ("I want in") plus a `turn` variable to break ties when both threads want
  in simultaneously. Simple and provably correct - the standard textbook
  example.
- **`02_dekkersAlgorithm.c`**: the original (older, more complex) two-process
  solution that Peterson's algorithm was later found to simplify. Instead of
  one busy-wait condition, a thread that sees contention repeatedly checks
  whose `turn` it is, backing off (clearing its own flag) and re-declaring
  interest until the turn comes back around.
- **`03_bakeryAlgorithm.c`**: Lamport's Bakery algorithm generalizes the idea
  to N threads using a take-a-number scheme, like a deli counter. Each
  thread grabs a ticket one higher than the current max, then waits for
  every thread holding a lower (ticket, id) pair. No thread can starve: any
  waiting thread's ticket is only ever passed by new arrivals with strictly
  higher numbers.
- **`04_hardwareTestAndSetCompareAndSwap.c`**: contrasts the above with the
  modern approach - a spinlock built directly on one hardware atomic
  instruction (`atomic_flag`/test-and-set, or `atomic_compare_exchange`/CAS).
  One atomic variable and one instruction replace the multi-variable
  bookkeeping the software algorithms need.
- **Why software-only solutions were needed historically**: early CPUs (and
  many still, for certain instruction sets) had no single instruction that
  could atomically read-modify-write memory; mutual exclusion had to be
  constructed logically from separate loads/stores, relying only on the
  guarantee that a single load or store of a machine word is atomic.
- **Why they're rarely used now**: hardware atomic instructions
  (test-and-set, CAS, fetch-and-add) are universally available, cheaper
  (one instruction vs. a busy-wait loop over multiple shared variables), and
  simpler to reason about and compose into higher-level primitives
  (`pthread_mutex_t`, `atomic_int`, etc.). Software algorithms are taught for
  the theory, not used directly in production code.
- **Mutual exclusion / progress / bounded waiting** (see also
  `12_RaceConditionAndCriticalSection/NOTES.md` for the definitions):
  - All four demos here satisfy mutual exclusion and progress by
    construction (that's the point of the algorithms).
  - Peterson's and Dekker's provide bounded waiting for exactly two
    threads/processes - each design guarantees a waiting thread is passed
    at most once before winning the tie-break.
  - The Bakery algorithm provides bounded waiting for N threads via strict
    ticket ordering - a waiting thread is only overtaken by threads that
    arrived and chose their ticket concurrently, never repeatedly.
  - The hardware-CAS/test-and-set spinlock provides mutual exclusion and
    progress, but plain spinlocks offer no bounded-waiting guarantee on
    their own (an unlucky thread can in principle keep losing the race to
    reacquire) - real lock implementations add ticket/queueing logic (e.g.
    ticket locks, MCS locks) on top of the hardware primitive to bound
    waiting, similar in spirit to the Bakery algorithm's approach.
