# UDP Patterns

Basic UDP client/server: `../53_UDPSockets`. TCP comparison: `../69_TcpDeepDive`.

## What UDP is
- Connectionless, unreliable datagrams over IP: 8-byte header (ports, length,
  checksum). Message BOUNDARIES are preserved; there is NO retransmission,
  ordering, duplicate suppression, flow or congestion control.
- Max payload: 65,507 bytes (IPv4), but anything above path MTU (~1472 over
  Ethernet) is IP-fragmented; losing one fragment loses the whole datagram.
  Keep datagrams <= ~1200-1400 bytes (or do PMTU discovery).
- `recvfrom` returns one datagram; a buffer smaller than it TRUNCATES the rest
  silently (`MSG_TRUNC` returns the real size). `recv` of 0 bytes is a valid empty datagram.
- "Connected" UDP (`connect()` on a datagram socket): sets the default peer,
  filters other senders, allows `send/recv`, and surfaces ICMP port-unreachable
  as `ECONNREFUSED`. No handshake happens.
- Socket buffer overflow silently drops datagrams (`netstat -su` "receive buffer
  errors"); raise `SO_RCVBUF`, drain faster, use `recvmmsg` for batching.

## When to choose UDP
Latency over reliability (VoIP, games, live video), DNS (small request/reply),
discovery/broadcast/multicast (mDNS, SSDP), telemetry/metrics (StatsD), custom
transports (QUIC/HTTP-3, WireGuard, RTP), and cases where TCP's head-of-line
blocking hurts.

## Building reliability on UDP (`01_requestRetryAndBoundaries.c`)
- Request id + timeout + retry with exponential backoff (+ jitter); make
  operations idempotent or dedupe by id on the server.
- Sequence numbers, ACKs/NACKs, sliding window, selective retransmit; FEC for
  lossy media; congestion control (otherwise you can congest the network and get
  rate-limited - be a good citizen: QUIC implements CUBIC/BBR in user space).
- Application-level checksum if you don't trust the weak 16-bit UDP checksum
  (optional in IPv4 - always on in IPv6).
- Amplification risk: small spoofed request -> large reply (DNS/NTP/memcached
  reflection DDoS). Rate-limit, require tokens/cookies (QUIC address validation).

## One-to-many (`02_broadcastAndMulticast.c`)
- **Broadcast:** `SO_BROADCAST`; limited (255.255.255.255) or directed (subnet);
  every host on the segment processes it; not routed; IPv6 has none.
- **Multicast:** group addresses 224.0.0.0/4 (IPv4), ff00::/8 (IPv6); receivers
  `IP_ADD_MEMBERSHIP` (IGMP tells the switch/router); sender controls
  `IP_MULTICAST_TTL`, `IP_MULTICAST_IF`, `IP_MULTICAST_LOOP`. Efficient fan-out
  (one packet on the wire, replicated by the network). Needs multicast-capable
  routing; often blocked on WiFi/cloud networks.
- **Anycast:** same IP announced from many places (BGP) - DNS roots, CDNs.

## Socket options worth knowing
`SO_REUSEADDR`/`SO_REUSEPORT` (multiple receivers; kernel load-balances unicast
across `REUSEPORT` sockets), `SO_RCVBUF`, `SO_RCVTIMEO`, `IP_PKTINFO` (which
local address/interface a datagram arrived on), `IP_MTU_DISCOVER`/`IP_DONTFRAG`,
`SO_TIMESTAMPING` (hardware timestamps - PTP/finance), `UDP_SEGMENT` (GSO) and
`recvmmsg/sendmmsg` for high packet rates.

## Senior interviewer Q&A
**Q: TCP vs UDP - how do you choose?**
A: TCP for ordered reliable streams where throughput and correctness dominate
(HTTP, databases, file transfer). UDP when you need low latency, tolerate or
handle loss yourself, need multicast/broadcast, or want to avoid head-of-line
blocking. Many modern protocols (QUIC) layer reliability on UDP.

**Q: How do you make a request/response protocol over UDP reliable?**
A: Unique request id, per-request timeout and bounded retries with exponential
backoff and jitter, server-side deduplication/idempotency, response matching by
id, optional window/ACK scheme for bulk data, and congestion control.

**Q: Why might my UDP receiver lose packets even on loopback?**
A: The socket receive buffer fills when the app is slower than the sender
(`SO_RCVBUF` default ~200 KB); kernel drops silently. Increase the buffer,
batch with `recvmmsg`, use multiple sockets/threads with `SO_REUSEPORT`, pin
threads, and check `/proc/net/udp` drops and `netstat -su`.

**Q: What is the max safe UDP payload and why?**
A: Stay under the path MTU minus headers (1472 on 1500-byte Ethernet; ~1200
for IPv6/QUIC safety); larger datagrams fragment and any lost fragment drops
the datagram.

**Q: How does UDP hole punching/NAT traversal work?**
A: Both peers send to each other via a rendezvous server to learn public
endpoints and open NAT mappings, then send directly; STUN/ICE formalize it,
TURN relays when it fails. NATs expire UDP mappings quickly - send keepalives.

**Q: Can UDP be used for DNS responses larger than 512 bytes?**
A: Classic DNS limits UDP to 512 B, then falls back to TCP (flag TC=1); EDNS(0)
raises it (~1232 recommended) to avoid fragmentation; DoT/DoH use TCP/TLS.
