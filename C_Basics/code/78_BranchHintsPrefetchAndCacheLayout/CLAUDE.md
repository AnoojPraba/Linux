# 78_BranchHintsPrefetchAndCacheLayout

Microarchitecture-aware C: branch prediction and likely/unlikely hints, array-of-structures vs structure-of-arrays, and software prefetch, measured with timers.

## Files
- `01_likely_and_branches.c` - sum of values >= 128 over random vs sorted data (same work, different speed); likely/unlikely macros via __builtin_expect
- `02_aos_vs_soa_prefetch.c` - AoS vs SoA scans over 4M elements and __builtin_prefetch on an index-driven gather
- `NOTES.md` - branch hints, prefetch, cache-friendly layout and "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_likely_and_branches.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/78_BranchHintsPrefetchAndCacheLayout/` (git-ignored).

## Key concepts / interview angles
- Unpredictable branches cost ~10-20 cycles on a mispredict; sorting data or going branchless removes it.
- `__builtin_expect` mostly affects code layout; measure before trusting hints; PGO is better.
- SoA wastes no cache-line bandwidth when only one field is scanned (AoS drags unused fields through the cache).
- Prefetch only helps with predictable-but-irregular access and sufficient distance; it can hurt.
- Related: false sharing and cache-line alignment.

## Gotchas
- Timings are machine-specific (this Raspberry Pi, aarch64); compare ratios, not absolute numbers.
- Uses 4M-element arrays (tens of MB); compile with `-O2` to see realistic effects (the Makefile does not pass -O).

## Related
- `../59_MemoryAlignmentAndPadding`
- `../../../OS/code/39_FalseSharing`
- `../../../OS/code/40_CacheCoherenceMESI`
- `../69_PerfAndStrace`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
