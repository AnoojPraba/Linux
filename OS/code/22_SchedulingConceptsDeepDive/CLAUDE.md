# 22_SchedulingConceptsDeepDive

Conceptual scheduling vocabulary: long/medium/short-term schedulers, dispatcher vs scheduler, preemption, starvation and aging (NOTES-only).

## Files
- `NOTES.md` - schedulers by timescale, dispatcher vs scheduler, preemptive vs non-preemptive, starvation and aging, cross-references

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Long-term (admission), medium-term (swapping), short-term (CPU dispatch) schedulers.
- Dispatcher does the switch (context switch, mode change, jump); scheduler decides who; dispatch latency matters.
- Criteria: throughput, turnaround, waiting, response time, fairness.
- Aging prevents starvation under priority scheduling.

## Related
- `../25_CPUScheduling`
- `../26_AdvancedSchedulingAlgorithms`
- `../27_ContextSwitchMechanics`
- `../24_ProcessScheduling`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
