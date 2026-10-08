# 20_ControlFlowExtras

Less common control flow: goto for cleanup/nested-loop exit, assert and setjmp/longjmp.

## Files
- `01_goto.c` - goto to break out of nested loops in one jump
- `02_assert.c` - assert for invariants (assert(denominator != 0)); NDEBUG compiles it out
- `03_setjmpLongjmp.c` - non-local jump back to a setjmp point, the C analogue of exceptions

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_goto.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/20_ControlFlowExtras/` (git-ignored).

## Key concepts / interview angles
- goto is accepted for single-exit cleanup and leaving nested loops.
- assert checks programmer invariants, never user input; never put side effects inside it (NDEBUG removes them).
- longjmp skips intervening frames with no cleanup (leaks, locks) and locals modified after setjmp need `volatile`.

## Related
- `../76_ErrorHandlingAndCleanupPatterns` - goto cleanup
- `../82_CoroutinesInC` - another use of non-local control flow
- `../56_UnitTesting`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
