# 26_AdvancedSchedulingAlgorithms

Multilevel queue and feedback-queue schedulers plus real-time scheduling (rate-monotonic, earliest-deadline-first) with schedulability checks.

## Files
- `01_multilevelQueueScheduling.c` - fixed classes (system, interactive, batch) each with its own algorithm
- `02_multilevelFeedbackQueue.c` - processes demote across levels; quantum grows at lower levels
- `03_rateMonotonicScheduling.c` - static priority by shortest period; utilisation-bound test
- `04_earliestDeadlineFirst.c` - dynamic priority by nearest absolute deadline
- `NOTES.md` - MLQ vs MLFQ, real-time scheduling

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_multilevelQueueScheduling.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/26_AdvancedSchedulingAlgorithms/` (git-ignored).

## Key concepts / interview angles
- MLQ: permanent classification; MLFQ: adaptive, favours short/interactive jobs, aging prevents starvation.
- RMS is optimal among fixed-priority; schedulable if U <= n(2^(1/n) - 1) (about 69% as n grows).
- EDF is optimal on a uniprocessor with U <= 100% but behaves badly in overload.
- Know the difference between hard and soft deadlines.

## Related
- `../25_CPUScheduling`
- `../23_RTOSConceptsAndTaskScheduling`
- `../16_PriorityInversion`
- `../22_SchedulingConceptsDeepDive`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
