# Scheduling Concepts Deep Dive

Notes-only: this is the theory layer behind `23_ProcessScheduling`'s
`nice`/`/proc` inspection demo and `24_CPUScheduling`'s FCFS/SJF/Round
Robin/Priority algorithm implementations - it explains the surrounding
concepts those files assume rather than duplicating their code.

## Schedulers by timescale

- **Long-term scheduler (job scheduler)**: decides which jobs are admitted
  from a job pool into the ready queue at all - controls the *degree of
  multiprogramming* (how many processes exist in memory competing for the
  CPU). Runs relatively rarely. Mostly a batch-system concept; modern
  general-purpose OSes (Linux included) don't really have a distinct
  long-term scheduler - nearly everything that's created gets admitted.
- **Short-term scheduler (CPU scheduler)**: picks which *ready* process
  runs next on the CPU. Runs very frequently (potentially every few
  milliseconds, on every timer interrupt/blocking call/preemption). This
  is what `24_CPUScheduling`'s FCFS/SJF/Round-Robin/Priority algorithms
  implement the decision logic for.
- **Medium-term scheduler**: sits between the two - handles *swapping*, i.e.
  temporarily moving a process out of main memory to disk (suspending it)
  to reduce memory pressure or the multiprogramming level, then swapping it
  back in later. Effectively adds a "Ready/Suspended" and
  "Blocked/Suspended" pair of states to the classic diagram in
  `03_ProcessControlBlockAndStates`.

## Dispatcher vs scheduler

- The **scheduler** is the *policy* - it decides *which* ready process
  should run next (FCFS, SJF, Round Robin, Priority - `24_CPUScheduling`'s
  algorithms are all schedulers in this sense).
- The **dispatcher** is the *mechanism* - it actually performs the context
  switch to give the CPU to the process the scheduler chose: switching to
  kernel mode if needed, saving the outgoing process's registers/PC into
  its PCB, loading the incoming process's registers/PC from its PCB,
  switching to user mode, and jumping to the resumed instruction (see
  `26_ContextSwitchMechanics` for the mechanics in detail).
- **Dispatch latency**: the time the dispatcher takes to stop one process
  and start another. Pure overhead from the workload's point of view - a
  scheduler that picks the theoretically perfect process is still bad in
  practice if dispatch latency is too high.

## Preemptive vs non-preemptive scheduling

- **Non-preemptive (cooperative)**: once a process is given the CPU, it
  keeps it until it voluntarily yields - blocks on I/O, or terminates. The
  scheduler cannot forcibly interrupt a running process.
- **Preemptive**: the scheduler *can* forcibly take the CPU away from a
  running process - on a timer interrupt (quantum expiry) or when a
  higher-priority process becomes ready - and hand it to someone else.
- What `24_CPUScheduling` actually implements:
  - `01_fcfs.c` (FCFS) - **non-preemptive**: processes run to completion
    strictly in arrival order, no interruption logic at all.
  - `02_sjf.c` (SJF) - **non-preemptive**: picks the shortest remaining job
    each round but, as implemented, runs the chosen process to completion
    before picking again (the preemptive variant of SJF is called
    Shortest-Remaining-Time-First and isn't what's implemented here).
  - `03_roundRobin.c` - **preemptive**: explicitly caps each process's run
    to one `quantum`, then puts it back at the end of the ready queue if
    it isn't done - the canonical preemptive algorithm.
  - `04_priorityScheduling.c` - **non-preemptive as implemented**: picks
    the highest-priority ready process each round but then runs it to
    completion; a real preemptive-priority scheduler would instead
    interrupt a running low-priority process the instant a higher-priority
    one becomes ready. (Priority scheduling can be built either way -
    Linux's real-time priority classes are preemptive.)

## Starvation and aging

- **Starvation**: a process is perpetually denied the CPU because the
  scheduler always finds something "better" to run first - classic case:
  a low-priority process under a strict priority scheduler that keeps
  receiving a steady stream of higher-priority arrivals, so it never gets
  scheduled. `04_priorityScheduling.c`'s header comment calls this out
  explicitly as something its simplified version doesn't defend against.
- **Aging**: the standard fix - gradually increase a process's effective
  priority the longer it waits in the ready queue, so that even a
  low-priority process's priority eventually exceeds every competitor's
  and it gets scheduled. Guarantees an upper bound on worst-case wait time
  without abandoning priority scheduling altogether.

## Cross-references

- `23_ProcessScheduling` - the `nice`/`/proc` demo shows the *priority*
  input to these algorithms (nice value -> scheduling priority) without
  implementing the algorithms themselves.
- `24_CPUScheduling` - the FCFS/SJF/Round-Robin/Priority implementations
  this folder's preemptive/non-preemptive classification is based on.
- `03_ProcessControlBlockAndStates` - the Ready/Running/Waiting state
  diagram that the short-term scheduler and dispatcher operate over.
- `26_ContextSwitchMechanics` - the detailed mechanics of what the
  dispatcher does during a context switch.
