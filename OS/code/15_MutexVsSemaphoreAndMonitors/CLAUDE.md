# 15_MutexVsSemaphoreAndMonitors

Conceptual comparison of mutexes, semaphores and monitors (NOTES-only).

## Files
- `NOTES.md` - mutex vs semaphore (ownership, binary vs counting, use cases) and monitors

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- Mutex has an owner (only the locker unlocks) and may support priority inheritance; semaphore is a signalling counter with no owner.
- Binary semaphore is not a mutex: no ownership, no priority inheritance, so it invites priority inversion.
- Monitor = mutex + condition variables bundled with the data (Java `synchronized`, C++ class with a mutex member).
- Use a mutex for mutual exclusion, a semaphore for resource counting or event signalling.

## Related
- `../09_Threads/04_semaphore.c`
- `../16_PriorityInversion`
- `../14_SyncProblems`
- `../21_AdvancedSyncPrimitives`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
