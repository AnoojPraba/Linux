# 80_SignalSafetyAndThreadLocal

Async-signal-safe handler rules and thread-local storage (_Thread_local) with pthread_once, plus a senior Q&A.

## Files
- `01_signal_safe.c` - handler restricted to async-signal-safe calls (write, _exit...), flag set via sig_atomic_t, driven by raise(SIGUSR1)
- `02_thread_local_once.c` - _Thread_local per-thread copy (errno analogy) and one-time init
- `NOTES.md` - signal safety, thread-local storage, one-time initialisation, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_signal_safe.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/80_SignalSafetyAndThreadLocal/` (git-ignored).

## Key concepts / interview angles
- A handler can interrupt anywhere, even inside malloc/printf holding a lock; only async-signal-safe functions are allowed (see `man 7 signal-safety`).
- Typical pattern: set a `volatile sig_atomic_t` flag (or write to a self-pipe) and handle it in the main loop.
- `errno` must be saved/restored in handlers.
- `_Thread_local` (C11) / `__thread` (GNU) gives per-thread copies.
- `pthread_once` / `call_once` for race-free lazy init.

## Related
- `../../../OS/code/06_SignalHandling`
- `../../../OS/code/09_Threads`
- `../16_ConstVolatile`
- `../76_ErrorHandlingAndCleanupPatterns`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
