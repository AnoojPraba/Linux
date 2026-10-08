# 25_CPUScheduling

Basic CPU scheduling algorithms simulated on a process set: FCFS, SJF, Round Robin, priority.

## Files
- `01_fcfs.c` - arrival order, convoy effect, waiting/turnaround times
- `02_sjf.c` - non-preemptive shortest job first among arrived processes
- `03_roundRobin.c` - fixed quantum, preempt and requeue
- `04_priorityScheduling.c` - highest priority (lowest number) first; same loop shape as SJF

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_fcfs.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/25_CPUScheduling/` (git-ignored).

## Key concepts / interview angles
- FCFS: simple but suffers the convoy effect.
- SJF minimises average waiting time but needs burst prediction and starves long jobs; preemptive version is SRTF.
- Round Robin: quantum trade-off (too small = switch overhead, too large = FCFS-like).
- Priority: starvation fixed by aging.
- Compute waiting = turnaround - burst; know Gantt-chart hand calculation.

## Related
- `../26_AdvancedSchedulingAlgorithms`
- `../22_SchedulingConceptsDeepDive`
- `../27_ContextSwitchMechanics`
- `../19_StarvationLivelockAndDeadlockPrevention`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
