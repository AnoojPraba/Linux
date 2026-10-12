# Sleep, Raw Key Input and a Minimal TCP Pair

- `sleep(s)` whole seconds; `usleep` obsolete; **`nanosleep`** (POSIX) takes a `timespec` and returns the remaining time if a signal interrupts it - loop on it. Sleeping takes the thread off the run queue (no busy wait); the delay is *at least* the request.
- Measure elapsed time with `clock_gettime(CLOCK_MONOTONIC)` (never goes backwards); `time()`/wall clock can jump.
- **`getch`/`getche` are Windows `<conio.h>`**, not standard C or POSIX. On Linux clear `ICANON` (no line buffering) and `ECHO` in `termios`, read one byte, then **restore** the old settings (otherwise the shell is left broken). `getchar` is line-buffered; return `int` so `EOF` is distinguishable.
- **TCP server:** `socket` -> `bind` -> `listen` -> `accept` (new fd per connection) -> `recv`/`send` -> `close`. **Client:** `socket` -> `connect` -> `send`/`recv` -> `close`. TCP is a byte stream: one `send` can arrive as several `recv`s; real protocols need framing (length prefix/delimiter) and a loop; `recv` returns 0 on orderly close.
- Port 0 lets the kernel pick; `getsockname` reports it. `SO_REUSEADDR` allows restart without waiting for TIME_WAIT. Network byte order: `htons`/`htonl`.
- Blocking `accept` serves one client; scale with fork/threads or `select`/`poll`/`epoll` (see `../../OS/code/54_IOMultiplexing`, `67_EpollInDepth`).
