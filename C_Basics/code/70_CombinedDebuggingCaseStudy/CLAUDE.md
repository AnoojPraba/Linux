# 70_CombinedDebuggingCaseStudy

Case study tying gdb, ASan and TSan together on one program with two independent seeded bugs.

## Files
- `01_buggyCounter.c` - bug 1: off-by-one heap overflow in buildRecords (ASan); bug 2: unsynchronised shared counter (TSan)
- `NOTES.md` - walkthrough: noticing the problem, gdb first look, ASan pinpointing, TSan pinpointing

## Build and run
- ASan: `gcc -Wall -Wextra -std=gnu11 -g -fsanitize=address -pthread 01_buggyCounter.c -o /tmp/x && /tmp/x`.
- TSan: same with `-fsanitize=thread` (do not combine with ASan). Follow NOTES.md for how to trigger each bug.
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/70_CombinedDebuggingCaseStudy/` (git-ignored).

## Key concepts / interview angles
- Pick the tool per symptom: crash -> gdb/core, memory corruption -> ASan/valgrind, nondeterminism -> TSan/Helgrind.
- Seed independent bugs so each tool run isolates one defect.
- Reproduce, minimise, then fix and re-run under sanitizers.

## Gotchas
- Both bugs are intentional; do not fix them.

## Related
- `../66_ThreadSanitizerDemo`
- `../67_GdbWorkflow`
- `../68_ValgrindAndAsan`
- `../../../OS/code/72_PerformanceDebuggingMethodology`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
