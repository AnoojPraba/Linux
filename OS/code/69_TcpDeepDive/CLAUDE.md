# 69_TcpDeepDive

TCP details interviewers ask about: socket buffer options and backpressure on loopback, and byte-stream framing with a correct write-all/read-exact, with a senior Q&A.

## Files
- `01_socketOptionsAndBuffers.c` - SO_SNDBUF/SO_RCVBUF (Linux doubles the value), non-blocking sender hitting EAGAIN, other options on an ephemeral loopback port
- `02_writeAll.c` - TCP is a byte stream: writeAll/readExact loops handle short reads and writes
- `NOTES.md` - connection lifecycle, reliability and flow control, framing, socket option cheat sheet, debugging toolbox, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_socketOptionsAndBuffers.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/69_TcpDeepDive/` (git-ignored).

## Key concepts / interview angles
- Short writes and reads are normal: loop until done and handle EINTR/EAGAIN.
- Message framing is the application's job (length prefix, delimiter).
- Flow control (receive window) vs congestion control (cwnd); a full send buffer is backpressure.
- TIME_WAIT purpose and `SO_REUSEADDR`; `TCP_NODELAY` vs Nagle; keepalive.
- Debugging: `ss -ti`, `tcpdump`, `strace`.

## Gotchas
- Binds loopback with an ephemeral port (getsockname); no external services needed.

## Related
- `../52_SocketProgramming`
- `../51_NetworkStackBasics`
- `../73_ConcurrentTcpServers`
- `../67_EpollInDepth`
- `../../../SystemDesign/topics/11_DNSAndTLSBasics`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
