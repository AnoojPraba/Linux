# 07_ErrnoAndErrorHandling

Correct errno usage: only inspect it after a call has reported failure.

## Files
- `01_errnoBasics.c` - open() on a nonexistent file; check the return value first, then errno, strerror/perror
- `NOTES.md` - errno and error handling

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_errnoBasics.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/07_ErrnoAndErrorHandling/` (git-ignored).

## Key concepts / interview angles
- errno is meaningful only after a failing call; successful calls may leave it nonzero.
- errno is thread-local (per-thread lvalue), so it is safe across threads but must be saved before calling other functions that may clobber it (e.g. in signal handlers).
- `perror`/`strerror` (use `strerror_r`/`%m` in threaded code).
- EINTR handling: retry `read`/`write` or use SA_RESTART; EAGAIN/EWOULDBLOCK for non-blocking I/O.

## Related
- `../08_SystemCalls/02_errnoAndPerror.c`
- `../06_SignalHandling`
- `../../../C_Basics/code/76_ErrorHandlingAndCleanupPatterns`
- `../../../C_Basics/code/80_SignalSafetyAndThreadLocal`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
