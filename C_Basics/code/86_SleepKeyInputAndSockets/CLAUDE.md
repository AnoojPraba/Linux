# 86_SleepKeyInputAndSockets

Pausing a program, reading a key without Enter (termios) and a minimal TCP client/server over loopback.

## Files
- `01_nanosleepAndRawKey.c` - nanosleep restarted after EINTR, elapsed-time check with CLOCK_MONOTONIC, and raw single-key input via termios (conio getch equivalent).
- `02_tcpLoopbackClientServer.c` - socket/bind/listen/accept server and connect/send/recv client in one program (fork), port chosen by the kernel.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_nanosleepAndRawKey.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/86_SleepKeyInputAndSockets/` (git-ignored).

## Key concepts / interview angles
- `sleep(s)` whole seconds; `usleep` obsolete; **`nanosleep`** (POSIX) takes a `timespec` and returns the remaining time if a signal interrupts it - loop on it. Sleeping takes the thread off the run queue (no busy wait); the delay is *at least* the request.
- Measure elapsed time with `clock_gettime(CLOCK_MONOTONIC)` (never goes backwards); `time()`/wall clock can jump.
- **`getch`/`getche` are Windows `<conio.h>`**, not standard C or POSIX. On Linux clear `ICANON` (no line buffering) and `ECHO` in `termios`, read one byte, then **restore** the old settings (otherwise the shell is left broken). `getchar` is line-buffered; return `int` so `EOF` is distinguishable.
- **TCP server:** `socket` -> `bind` -> `listen` -> `accept` (new fd per connection) -> `recv`/`send` -> `close`. **Client:** `socket` -> `connect` -> `send`/`recv` -> `close`. TCP is a byte stream: one `send` can arrive as several `recv`s; real protocols need framing (length prefix/delimiter) and a loop; `recv` returns 0 on orderly close.
- Port 0 lets the kernel pick; `getsockname` reports it. `SO_REUSEADDR` allows restart without waiting for TIME_WAIT. Network byte order: `htons`/`htonl`.

## Gotchas
- The key demo only reads a key when stdin is a terminal; otherwise it prints a fixed code so it can run in a pipeline.
- Check every return value; ignore SIGPIPE or use MSG_NOSIGNAL when writing to a possibly closed socket.
