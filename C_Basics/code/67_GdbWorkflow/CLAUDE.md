# 67_GdbWorkflow

GDB workflow against a deliberately crashing program: breakpoints, watchpoints, backtraces, memory/registers, core dumps and attach.

## Files
- `01_segfaultDemo.c` - NULL record dereference triggered by an out-of-range lookup index (argv[1], default 1)
- `NOTES.md` - breakpoints and watchpoints, backtrace and frames, examining variables/memory, stepping, core dumps, registers, attaching to a process (146 lines)

## Build and run
- `gcc -Wall -Wextra -std=gnu11 -g -O0 01_segfaultDemo.c -o /tmp/x && gdb --args /tmp/x 9` (pass an out-of-range index to crash; default index 1 is fine).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/67_GdbWorkflow/` (git-ignored).

## Key concepts / interview angles
- Compile with `-g -O0` for usable variables; `bt`, `frame N`, `info locals`, `x/16xb`, `p *ptr`.
- Watchpoints (`watch var`) break on write; hardware watchpoints are limited in number.
- Post-mortem: `ulimit -c unlimited`, `gdb prog core`.
- `gdb -p PID` attaches to a live process (may need ptrace permission).

## Gotchas
- The segfault is intentional; do not add a NULL check to the demo.

## Related
- `../68_ValgrindAndAsan`
- `../70_CombinedDebuggingCaseStudy`
- `../79_StackFramesAndCallingConvention`
- `../../../OS/code/72_PerformanceDebuggingMethodology`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
