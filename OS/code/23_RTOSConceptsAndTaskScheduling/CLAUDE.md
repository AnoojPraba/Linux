# 23_RTOSConceptsAndTaskScheduling

Conceptual notes on RTOS design: tasks, deterministic scheduling, RTOS primitives, per-task stack sizing and WCET analysis (NOTES-only; embedded interview material).

## Files
- `NOTES.md` - RTOS vs general-purpose OS, task model, RTOS primitives, stack sizing per task, real-world examples, WCET analysis

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Determinism and bounded latency matter more than throughput; hard vs soft real time.
- Fixed-priority preemptive scheduling with rate-monotonic priorities is the default model (see `../26_AdvancedSchedulingAlgorithms`).
- Primitives: queues, semaphores, mutexes with priority inheritance, event flags, timers, ISR-to-task signalling.
- Each task has its own stack: size from worst-case depth plus ISR frames; watch for overflow.
- WCET analysis feeds schedulability tests (utilisation bound, response-time analysis).

## Related
- `../16_PriorityInversion`
- `../26_AdvancedSchedulingAlgorithms`
- `../43_EmbeddedReliabilityAndPowerManagement`
- `../11_VolatileVsAtomicEmbedded`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
