# Advanced Scheduling Algorithms

Builds on `22_SchedulingConceptsDeepDive`'s theory layer and
`24_CPUScheduling`'s FCFS/SJF/Round-Robin/Priority implementations, covering
two further categories of scheduling: multi-queue scheduling for general-
purpose systems, and static/dynamic-priority real-time scheduling.

## Multilevel Queue (MLQ) vs Multilevel Feedback Queue (MLFQ)

- **Multilevel Queue (`01_multilevelQueueScheduling.c`)**: processes are
  split into separate ready queues by category (e.g. system, interactive,
  batch), each queue can run its own algorithm internally, and a fixed
  priority (or a time-slice split) decides which queue gets the CPU between
  queues. Once a process is assigned to a queue, it stays there forever.
  Simple, and gives strong guarantees to high-priority categories, but
  requires knowing a process's category up front and can't adapt if that
  classification turns out to be wrong or changes over time.
- **Multilevel Feedback Queue (`02_multilevelFeedbackQueue.c`)**: same
  multiple-queues idea, but processes can move between levels based on
  observed behavior instead of a fixed assignment. A process that
  repeatedly uses its entire time quantum without blocking looks CPU-bound
  and gets demoted to a lower-priority, longer-quantum level; a process
  that blocks for I/O quickly (e.g. waiting on user input) looks
  interactive and stays at (or is promoted back toward) the high-priority,
  short-quantum level.
- **MLFQ's key advantage over MLQ**: the scheduler does not need to know a
  process's category upfront - it discovers CPU-bound vs I/O-bound behavior
  purely from how the process actually runs, and continuously re-classifies
  it as that behavior changes. This is what real general-purpose scheduler
  implementations approximate (the historical BSD/Solaris multilevel
  feedback queues, and conceptually Linux's older O(1) scheduler's
  interactivity heuristics, before CFS's fully different vruntime-based
  approach).

## Real-time scheduling

Real-time scheduling cares about meeting deadlines, not just minimizing
average waiting time - a periodic task that runs one cycle late can be a
correctness failure (e.g. a missed sensor sampling deadline), not just a
performance hit.

- **Rate Monotonic Scheduling (RMS)**: a static-priority algorithm for
  periodic tasks - each task is assigned a fixed priority based on its
  period, with a SHORTER period getting a HIGHER priority (it must run more
  often, so it's given precedence). Priorities never change at runtime.
  RMS is *optimal among static-priority algorithms*: if any static-priority
  algorithm can schedule a given periodic task set to meet all deadlines,
  RMS can too. It has a known sufficient (not exactly necessary) utilization
  bound for guaranteeing schedulability with n tasks: U <= n(2^(1/n) - 1),
  which converges to about 69% as n grows.
- **Earliest Deadline First (EDF)**: a dynamic-priority algorithm - at any
  point, whichever ready task has the nearest ABSOLUTE deadline runs next;
  priorities are recomputed continuously as deadlines approach. EDF is
  *optimal among all scheduling algorithms* (not just static-priority ones)
  for meeting deadlines: it can achieve up to 100% CPU utilization and still
  meet every deadline, as long as total utilization stays <= 100%.
- **Trade-off**: EDF's optimality comes at the cost of implementation
  complexity - every task's priority must be dynamically recomputed as
  deadlines shift, versus RMS's priorities being fixed once at design time.
  EDF also degrades poorly under overload: once utilization exceeds 100%,
  EDF can suffer a "domino effect" where many tasks miss their deadlines
  together, because priority order keeps shifting to chase whichever
  deadline is nearest even among tasks that are already going to miss.
  RMS degrades more gracefully under overload: since priorities are fixed,
  the same lower-priority tasks reliably miss deadlines first while
  higher-priority (shorter-period) tasks keep meeting theirs, which is
  often preferable in practice even though RMS's guaranteed utilization
  bound is lower than EDF's.
