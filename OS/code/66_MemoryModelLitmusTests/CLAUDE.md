# 66_MemoryModelLitmusTests

Memory-model litmus tests (message passing, store buffering), the ABA problem with a tagged Treiber stack, and safe memory reclamation, with a senior Q&A.

## Files
- `01_litmusMpSb.c` - runs MP and SB patterns millions of times under relaxed and stronger orders and counts observed outcomes
- `02_abaTaggedStack.c` - Treiber stack with index+tag packed into a 64-bit CAS word; shows ABA and the tag fix
- `NOTES.md` - what a memory model gives you, the two litmus tests, ABA, safe memory reclamation (hazard pointers, epochs), "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_litmusMpSb.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/66_MemoryModelLitmusTests/` (git-ignored).

## Key concepts / interview angles
- MP: data then flag; without release/acquire a reader can see the flag but stale data (allowed on ARM, forbidden on x86 TSO).
- SB (store buffering): both threads can read 0 even on x86 unless seq_cst/fences are used.
- ABA: CAS succeeds although the value changed and changed back; fix with tags/versions or hazard pointers/RCU/epoch reclamation.
- Weak outcomes are probabilistic; absence of an outcome in a run proves nothing.

## Gotchas
- Weak-ordering outcomes are far more likely on this aarch64 machine than on x86; counts vary run to run.
- Runs millions of iterations per test, so it takes a few seconds.

## Related
- `../20_Atomics`
- `../40_CacheCoherenceMESI`
- `../37_LockFreeRingBuffer`
- `../65_FutexAndSeqlock`
- `../../../Cpp/code/24_Concurrency/07_memoryOrderingLevels.cpp`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
