# 76_ErrorHandlingAndCleanupPatterns

C error-handling and resource-cleanup idioms: goto cleanup, GCC cleanup attribute and setjmp/longjmp, with a senior Q&A.

## Files
- `01_goto_cleanup.c` - single-exit idiom: acquire in order, release in reverse via fall-through labels
- `02_cleanup_attribute.c` - `__attribute__((cleanup(fn)))` as RAII-like scope exit (GCC/Clang extension, not standard C)
- `03_setjmp_longjmp.c` - non-local goto as poor man's exceptions
- `NOTES.md` - patterns, trade-offs and "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_goto_cleanup.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/76_ErrorHandlingAndCleanupPatterns/` (git-ignored).

## Key concepts / interview angles
- Goto cleanup is the kernel and systems idiom: one exit path, labels in reverse acquisition order.
- `cleanup` attribute gives scope-based release but is non-portable (C2y proposes `defer`).
- longjmp skips destructors/cleanup and has volatile-local pitfalls; avoid across resource ownership.
- Report errors via return codes plus errno or an out-parameter; never ignore return values.
- Make cleanup idempotent (free(NULL) is safe).

## Gotchas
- 02 needs GCC or Clang (non-standard attribute).

## Related
- `../20_ControlFlowExtras`
- `../15_DynamicMemory`
- `../../../OS/code/07_ErrnoAndErrorHandling`
- `../../../Cpp/code/14_RAII`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
