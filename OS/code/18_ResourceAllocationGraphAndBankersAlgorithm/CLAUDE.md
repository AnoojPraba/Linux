# 18_ResourceAllocationGraphAndBankersAlgorithm

Resource allocation graphs and the Banker's deadlock-avoidance algorithm (safety check and request handling).

## Files
- `01_bankersAlgorithm.c` - findSafeSequence() safety algorithm and requestResources() with a trial allocation; max/allocation/need matrices
- `NOTES.md` - RAG cycle detection and Banker's algorithm

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_bankersAlgorithm.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/18_ResourceAllocationGraphAndBankersAlgorithm/` (git-ignored).

## Key concepts / interview angles
- Safe state = exists an order in which every process can finish; unsafe is not the same as deadlocked.
- Request is granted only if the resulting state is still safe (tentatively allocate, run the safety check, roll back otherwise).
- RAG: a cycle means deadlock only for single-instance resources.
- Needs advance knowledge of maximum claims, so it is rare in real systems; O(m n^2).

## Related
- `../17_DeadlockDetectionAvoidance`
- `../19_StarvationLivelockAndDeadlockPrevention`
- `../14_SyncProblems`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
