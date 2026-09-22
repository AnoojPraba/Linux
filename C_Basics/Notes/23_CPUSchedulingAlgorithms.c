CPU scheduling decides which ready process/thread the CPU runs next.
`code/53_ProcessScheduling/01_niceAndProcInspection.c` covered the *hint*
a process gives the OS (`nice()`); these files simulate the actual
decision-making algorithms an OS scheduler chooses between, tracked via a
small process table (arrival time, burst time, computed waiting/turnaround
time).

Two numbers every algorithm below is evaluated by:
*   Waiting time - how long a process sat ready but not running.
*   Turnaround time - total time from arrival to completion (waiting time
    plus the time actually spent running).
Lower average waiting time generally means a more responsive scheduler.

*   FCFS (First-Come, First-Served) - run processes strictly in arrival
    order, each to completion. Simplest possible policy, but a short
    process arriving just after a long one still waits for the whole long
    burst to finish (the "convoy effect"). See
    `code/54_CPUScheduling/01_fcfs.c`.
*   SJF (Shortest Job First, non-preemptive) - among processes that have
    already arrived, always run the one with the smallest burst time next.
    Minimizes average waiting time versus FCFS, but requires knowing each
    burst time in advance (rarely true in practice) and can starve long
    processes if short ones keep arriving. See
    `code/54_CPUScheduling/02_sjf.c`.
*   Round Robin - each process gets a fixed time slice (the quantum), then
    is preempted and sent to the back of a queue if unfinished - unlike
    FCFS/SJF, which always run one process to completion once started.
    Bounds the worst-case wait any process experiences, at the cost of
    more context switches. See `code/54_CPUScheduling/03_roundRobin.c`.
*   Priority scheduling - always run the highest-priority ready process
    next; structurally identical to SJF's "always pick the best candidate"
    loop, just comparing priority instead of burst time. Real schedulers
    pair this with aging (temporarily boosting a long-waiting process's
    priority) to avoid starving low-priority work indefinitely - this
    simplified version doesn't implement aging. See
    `code/54_CPUScheduling/04_priorityScheduling.c`.
