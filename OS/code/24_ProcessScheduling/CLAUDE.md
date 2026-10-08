# 24_ProcessScheduling

Process priority in practice: nice values and reading scheduler state from /proc.

## Files
- `01_niceAndProcInspection.c` - prints nice(0), calls nice(5), reads the State line from /proc/self/status

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_niceAndProcInspection.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/24_ProcessScheduling/` (git-ignored).

## Key concepts / interview angles
- Nice ranges -20..19; only root can lower it; it is a weight/hint for CFS, not a guarantee.
- Linux CFS (and EEVDF in newer kernels) schedules by virtual runtime weighted by nice.
- Real-time classes SCHED_FIFO/SCHED_RR preempt normal tasks; `chrt`, `taskset` for control.
- `/proc/[pid]/status` and `/proc/[pid]/sched` expose scheduling data.

## Gotchas
- Calling `nice(5)` lowers this process's priority only; it does not persist after the program exits.

## Related
- `../03_ProcessControlBlockAndStates`
- `../25_CPUScheduling`
- `../22_SchedulingConceptsDeepDive`
- `../05_ResourceLimits`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
