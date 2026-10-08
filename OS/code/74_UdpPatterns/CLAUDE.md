# 74_UdpPatterns

UDP patterns beyond the basic echo: datagram boundaries, request/retry reliability and broadcast/multicast, with a senior Q&A.

## Files
- `01_requestRetryAndBoundaries.c` - preserved message boundaries and a request/response with timeout, retry and dedup over lossy UDP
- `02_broadcastAndMulticast.c` - SO_BROADCAST and multicast group join/send/receive on loopback
- `NOTES.md` - what UDP is, when to choose it, reliability on UDP, one-to-many, socket options, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_requestRetryAndBoundaries.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/74_UdpPatterns/` (git-ignored).

## Key concepts / interview angles
- UDP preserves boundaries but gives no reliability, ordering or congestion control; build ack/retry with sequence numbers and idempotent handlers.
- Broadcast stays on the LAN segment; multicast needs IGMP group membership (`IP_ADD_MEMBERSHIP`).
- Pick UDP for latency-sensitive or loss-tolerant traffic, DNS, QUIC underpinnings.
- Path MTU and fragmentation: keep datagrams under the MTU.

## Gotchas
- `02_broadcastAndMulticast.c` uses loopback multicast; if the interface cannot join a group it reports it and exits cleanly (comment in source).
- Uses ephemeral ports; no fixed port required.

## Related
- `../53_UDPSockets`
- `../52_SocketProgramming`
- `../69_TcpDeepDive`
- `../../../SystemDesign/topics/11_DNSAndTLSBasics`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
