# 72_PerformanceDebuggingMethodology

A "the service is slow" methodology demo: two accidental slowdowns with fixes, timed side by side, plus triage notes and a senior Q&A.

## Files
- `01_slowService.c` - `slow` vs `fast` modes: bug 1: `strlen()` in the loop condition makes a loop O(n^2); bug 2: one `write()` syscall per tiny record vs a batched buffer; timed side by side
- `NOTES.md` - workflow, triage by symptom, profiling tools and what they tell you, typical C-level culprits, "Senior interviewer Q&A"

## Build and run
- Pick a mode: `gcc -Wall -Wextra -std=gnu11 -g 01_slowService.c -o /tmp/x && /tmp/x slow` or `/tmp/x fast` (default is slow).
- Profile: `strace -c /tmp/x slow` (syscall-count signature), `perf record -g /tmp/x slow` then `perf report`.
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_slowService.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/72_PerformanceDebuggingMethodology/` (git-ignored).

## Key concepts / interview angles
- Method: define the symptom and metric, measure before changing, find the bottleneck (CPU, memory, I/O, lock, network), fix one thing, re-measure.
- USE method (utilisation, saturation, errors) and the 60-second checklist (`uptime`, `vmstat`, `iostat`, `top`, `ss`).
- Sampling profilers (perf) vs tracing (strace) vs counters; flame graphs.
- Typical C culprits: O(n^2) string ops, unbuffered/tiny syscalls, lock contention, cache misses.

## Gotchas
- Timings are machine-specific (aarch64 Raspberry Pi); compare slow vs fast ratios.
- perf/strace must be installed.

## Related
- `../71_EbpfAndTracingBasics`
- `../../../C_Basics/code/69_PerfAndStrace`
- `../../../C_Basics/code/70_CombinedDebuggingCaseStudy`
- `../../../SystemDesign/topics/21_ObservabilityLogsMetricsTraces`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
