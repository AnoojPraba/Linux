# 38_ConcurrentDataStructures

Lock-striped concurrent hash map: splitting one global lock into per-stripe locks to reduce contention.

## Files
- `01_lockStripedHashMap.c` - bucket array divided into stripes, each with its own mutex; threads on different stripes do not block each other
- `NOTES.md` - lock striping and a note on concurrent skip lists (conceptual only, no 02_ file)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_lockStripedHashMap.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/38_ConcurrentDataStructures/` (git-ignored).

## Key concepts / interview angles
- A single global mutex serialises all operations; striping by `hash(key) % stripes` allows parallelism across unrelated keys.
- Whole-map operations (resize, size, iteration) need all stripe locks in a fixed order.
- Choose stripes >= core count; false sharing between adjacent lock words matters.
- Alternatives: reader/writer locks, per-bucket locks, lock-free maps, RCU, `ConcurrentHashMap`-style designs.
- Concurrent skip lists are the ordered-map analogue (conceptual).

## Related
- `../21_AdvancedSyncPrimitives`
- `../39_FalseSharing`
- `../37_LockFreeRingBuffer`
- `../../../C_Basics/code/34_HashTable`
- `../../../Cpp/code/24_Concurrency/08_threadSafeLruCacheLockStriped.cpp`
- `../../../SystemDesign/topics/26_DesignCaseStudyConcurrentInMemoryKeyValueStore`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
