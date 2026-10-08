# 16_PriorityInversion

Priority inversion (the Mars Pathfinder bug) with an illustrative three-thread demo and notes on priority inheritance/ceiling.

## Files
- `01_priorityInversionDemo.c` - low, medium and high priority threads sharing a lock; tries SCHED_FIFO via pthread_setschedparam
- `NOTES.md` - what priority inversion is, Pathfinder, mitigations

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_priorityInversionDemo.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/16_PriorityInversion/` (git-ignored).

## Key concepts / interview angles
- A high-priority task blocks on a lock held by a low-priority one; a medium-priority task preempts the holder, so high effectively waits on medium.
- Fixes: priority inheritance (`PTHREAD_PRIO_INHERIT`), priority ceiling, avoiding shared locks, disabling preemption in short critical sections.
- Mars Pathfinder 1997: watchdog resets until priority inheritance was enabled.
- Only meaningful with a real-time scheduling policy (SCHED_FIFO/RR).

## Gotchas
- SCHED_FIFO needs root or CAP_SYS_NICE; without it the program prints a note, runs at default priority and the inversion will not reproduce reliably (the demo is illustrative, as its own comments say).
- Uses sleeps/spins totalling about a second.

## Related
- `../15_MutexVsSemaphoreAndMonitors`
- `../23_RTOSConceptsAndTaskScheduling`
- `../25_CPUScheduling`
- `../26_AdvancedSchedulingAlgorithms`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
