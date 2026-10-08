# 19_StarvationLivelockAndDeadlockPrevention

Conceptual notes on starvation, livelock and the four ways to prevent deadlock (NOTES-only).

## Files
- `NOTES.md` - deadlock prevention (attack each Coffman condition), starvation vs livelock

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Prevention attacks one condition: all-at-once acquisition (hold-and-wait), preemption, global ordering (circular wait).
- Starvation: perpetually denied a resource (unfair scheduling, reader preference); fix with aging or fair queues.
- Livelock: threads keep running but make no progress (both back off the same way); fix with randomised backoff.
- Distinguish all three in interviews: deadlock blocks, livelock spins, starvation is unfairness.

## Related
- `../17_DeadlockDetectionAvoidance`
- `../18_ResourceAllocationGraphAndBankersAlgorithm`
- `../26_AdvancedSchedulingAlgorithms`
- `../14_SyncProblems`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
