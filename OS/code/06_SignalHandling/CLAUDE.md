# 06_SignalHandling

Basic POSIX signal handling: installing a handler, raise() and alarm()/pause().

## Files
- `01_customHandler.c` - sigaction handler for SIGINT sets a flag; main loop sleeps up to 5 s checking it (send SIGINT or press Ctrl+C)
- `02_alarmRaise.c` - signal(SIGALRM) handler; alarm(N) then pause() waits for it; raise() sends a signal to self

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_customHandler.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/06_SignalHandling/` (git-ignored).

## Key concepts / interview angles
- A handler can run at almost any instruction: only async-signal-safe calls, set a `volatile sig_atomic_t` flag.
- Prefer `sigaction` over `signal` (portable semantics, SA_RESTART, masks).
- SIGKILL and SIGSTOP cannot be caught.
- Standard signals coalesce; real-time signals queue.
- Alternatives: signalfd, self-pipe trick.

## Gotchas
- Programs wait seconds (sleep/alarm/pause); `02_alarmRaise.c` blocks in `pause()` until the alarm fires.

## Related
- `../../../C_Basics/code/80_SignalSafetyAndThreadLocal`
- `../04_ProcessLifecycle`
- `../08_SystemCalls`
- `../07_ErrnoAndErrorHandling`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
