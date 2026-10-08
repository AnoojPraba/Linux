# 53_UDPSockets

Minimal UDP datagram server and client over loopback on port 8081.

## Files
- `01_udpServer.c` - SOCK_DGRAM socket, bind, recvfrom (gets sender address)
- `02_udpClient.c` - sendto with an explicit destination, no connect()

## Build and run
- Run in two terminals: server `gcc -Wall -Wextra -std=gnu11 01_udpServer.c -o /tmp/srv && /tmp/srv`, then client `gcc -Wall -Wextra -std=gnu11 02_udpClient.c -o /tmp/cli && /tmp/cli`.
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_udpServer.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/53_UDPSockets/` (git-ignored).

## Key concepts / interview angles
- UDP: no connection, no ordering, no retransmission, message boundaries preserved; datagram may be lost or duplicated.
- Used for DNS, real-time media, games, QUIC.
- `connect()` on UDP merely fixes the peer address.
- For reliability add sequence numbers, acks and retries (see `../74_UdpPatterns`).

## Gotchas
- Uses fixed UDP port 8081 on loopback; the server blocks in `recvfrom` until the client sends, so start it first.

## Related
- `../52_SocketProgramming`
- `../74_UdpPatterns`
- `../51_NetworkStackBasics`
- `../../../SystemDesign/topics/11_DNSAndTLSBasics`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
