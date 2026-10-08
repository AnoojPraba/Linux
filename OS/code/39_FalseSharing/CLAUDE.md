# 39_FalseSharing

False sharing benchmark: independent per-thread counters in one cache line vs padded to separate lines.

## Files
- `01_falseSharingBenchmark.c` - 4 threads x 100,000,000 increments on unpadded vs 64-byte padded counters, timed

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_falseSharingBenchmark.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/39_FalseSharing/` (git-ignored).

## Key concepts / interview angles
- Threads writing different variables on the same cache line still invalidate each other (MESI ping-pong).
- Fix: pad/align hot per-thread data to the cache line (`alignas(64)`, `__attribute__((aligned(64)))`), or keep per-thread data thread-local.
- Cache line is 64 bytes on x86-64 and on this Cortex-A part (some CPUs use 128).
- Detect with `perf c2c` or by seeing poor scaling with threads.

## Gotchas
- Runs for several seconds (400 million increments across threads) and prints timings that are machine-specific (aarch64 Raspberry Pi).
- Compile with optimisation off (as the Makefile does) or the compiler may keep counters in registers and hide the effect.

## Related
- `../40_CacheCoherenceMESI`
- `../37_LockFreeRingBuffer`
- `../../../C_Basics/code/59_MemoryAlignmentAndPadding`
- `../../../C_Basics/code/78_BranchHintsPrefetchAndCacheLayout`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
