# 52_SocketProgramming

Minimal TCP client and server over loopback on port 8080.

## Files
- `01_tcpServer.c` - socket, SO_REUSEADDR, bind, listen(backlog 1), accept one client, receive and reply
- `02_tcpClient.c` - socket, inet_pton to 127.0.0.1, connect, send and receive

## Build and run
- Run in two terminals: `gcc -Wall -Wextra -std=gnu11 01_tcpServer.c -o /tmp/srv && /tmp/srv`, then `gcc -Wall -Wextra -std=gnu11 02_tcpClient.c -o /tmp/cli && /tmp/cli`.
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_tcpServer.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/52_SocketProgramming/` (git-ignored).

## Key concepts / interview angles
- Server sequence: socket, setsockopt(SO_REUSEADDR), bind, listen, accept; client: socket, connect.
- TCP is a byte stream: `recv` may return partial data; real protocols need framing.
- `htons`/`htonl` convert to network byte order (see C_Basics endianness).
- This server handles one client then exits; concurrency models are in `../73_ConcurrentTcpServers`.

## Gotchas
- Uses fixed TCP port 8080 on 127.0.0.1 (not ephemeral); the server blocks in `accept()` until the client connects, so start the server first.
- If the port is busy (another service on 8080), bind fails.

## Related
- `../53_UDPSockets`
- `../73_ConcurrentTcpServers`
- `../54_IOMultiplexing`
- `../69_TcpDeepDive`
- `../../../C_Basics/code/06_EndiannessAndByteOrder`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
