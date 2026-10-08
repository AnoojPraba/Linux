# 69_PerfAndStrace

Profiling and tracing tools (perf, strace, ltrace, /usr/bin/time) via a cache-friendly vs cache-hostile loop demo.

## Files
- `01_naiveVsOptimizedLoop.c` - sequential array sum (cache friendly) vs large-stride access, for measuring with perf
- `NOTES.md` - strace (syscalls), ltrace (library calls), perf stat, perf record/report, /usr/bin/time -v

## Build and run
- Build with `-g -O1` or the default flags, then `perf stat /tmp/x` or `strace -c /tmp/x`.
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_naiveVsOptimizedLoop.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/69_PerfAndStrace/` (git-ignored).

## Key concepts / interview angles
- `strace -c` summarises syscalls; `-f` follows threads/forks; `-e trace=` filters.
- `perf stat` gives cycles/instructions/cache-misses; `perf record -g` + `perf report` finds hot functions.
- Cache-friendly access order can be an order of magnitude faster at identical operation counts.
- `/usr/bin/time -v` reports max RSS and page faults.

## Gotchas
- Timings and counters are machine-specific (aarch64 Raspberry Pi here).
- perf, strace and ltrace must be installed; perf may be restricted by `kernel.perf_event_paranoid`.

## Related
- `../78_BranchHintsPrefetchAndCacheLayout`
- `../../../OS/code/72_PerformanceDebuggingMethodology`
- `../../../OS/code/71_EbpfAndTracingBasics`
- `../67_GdbWorkflow`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
