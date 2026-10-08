# 36_AdvancedHashingAndCacheAwareStructures

Robin Hood open-addressing hashing implemented with probe-distance tracking; cache-oblivious algorithms and Judy arrays covered conceptually.

## Files
- `01_robinHoodHashing.c` - open addressing that swaps entries so probe distances equalise; lookup can stop early
- `NOTES.md` - Robin Hood, cache-oblivious algorithms (concept), Judy arrays (concept)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_robinHoodHashing.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/36_AdvancedHashingAndCacheAwareStructures/` (git-ignored).

## Key concepts / interview angles
- Robin Hood: on insert, if the incoming entry is farther from its ideal slot than the resident one, swap ("steal from the rich").
- Low probe-length variance and early-termination of failed lookups using the stored `probeDistance`.
- Cache-oblivious algorithms perform well at every cache level without knowing sizes; cache-aware ones are tuned to a specific line/block size.
- Judy arrays: cache-optimised associative array; a "know it exists" topic.

## Related
- `../34_HashTable`
- `../78_BranchHintsPrefetchAndCacheLayout`
- `../../../OS/code/40_CacheCoherenceMESI`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
