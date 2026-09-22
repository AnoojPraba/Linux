# perf, strace, ltrace, and /usr/bin/time

Companion demo: `01_naiveVsOptimizedLoop.c` sums the same array two ways -
`sumSequential()` (cache-friendly) and `sumStrided()` (deliberately jumps far
on every access) - for `perf stat` to show a measurable difference.

## strace - trace syscalls

- `strace ./program` prints every syscall the process makes (`open`,
  `read`, `mmap`, `brk`, ...) with arguments and return values - the tool
  for "why is this program touching that file / blocking on that fd /
  failing with `ENOENT`."
- `strace -c ./program` instead prints a summary table (syscall name, call
  count, total time, error count) - useful for "which syscall is this
  spending its time in" without a giant per-call log.
- `strace -f` follows child processes after `fork()`/`clone()` too;
  `strace -e trace=open,read,write ./program` filters to just those calls.
- Typical output line: `open("/etc/passwd", O_RDONLY) = 3` - the syscall,
  its arguments, and its return value (here, the new fd).

## ltrace - trace library calls

- Same idea as `strace` but for dynamic library/libc calls (`malloc`,
  `strlen`, `printf`) instead of syscalls: `ltrace ./program`.
- Useful for "which library function is being called with what arguments"
  when the bug is above the syscall layer (e.g. a bad `malloc` size), but
  much less commonly available/maintained than `strace` on modern distros -
  `strace -e trace=memory` can substitute for basic allocation tracing.

## perf stat - aggregate hardware counters

- `perf stat ./01_naiveVsOptimizedLoop` prints wall time plus hardware
  counters (instructions, cycles, cache-references, cache-misses,
  branches, branch-misses) and a derived "insns per cycle" (IPC).
- Comparing the two loops: `sumStrided()`'s far jumps defeat the cache, so
  expect a much higher cache-miss count and lower IPC for the run than a
  version calling only `sumSequential()` - even though both do the same
  amount of arithmetic. This is the concrete evidence for "cache-friendly
  access patterns matter" beyond just Big-O complexity.
- `perf stat -e cache-misses,cache-references,instructions,cycles
  ./program` picks specific counters when the default set is too broad.

## perf record / perf report - find hot functions

- `perf record ./program` samples the call stack periodically (statistical
  profiling, near-zero overhead) and writes `perf.data`.
- `perf report` then shows a sorted list of functions by percentage of
  samples - the "where is the CPU time actually going" view, more precise
  than manually reasoning about which function should be hot.
- `perf record -g ...` captures call graphs so `perf report` can show
  *which caller* is driving time into a hot function, not just the
  function in isolation.

## /usr/bin/time -v - basic resource usage

- Note the explicit path: the shell-builtin `time` (bash's own) only
  prints wall/user/sys time; `/usr/bin/time -v ./program` is a separate
  binary with a much larger report: max resident set size, major/minor
  page faults, context switches (voluntary/involuntary), filesystem I/O
  counts.
- Good first check before reaching for `perf` - e.g. "did this leak
  memory" (rising max RSS across runs) or "is this thrashing" (high major
  page fault count) without needing full profiling.

## Quick reference: which tool for which question

- "What files/syscalls is it touching?" -> `strace`.
- "What library calls is it making?" -> `ltrace` (or `strace -e
  trace=memory` for allocation-only).
- "Is it CPU-bound, and where specifically?" -> `perf record` + `perf
  report` (hot functions), `perf stat` (aggregate counters/IPC).
- "How much memory/CPU/context-switching overall?" -> `/usr/bin/time -v`.
