# TCP in Depth (for systems interviews)

## Connection lifecycle
- **Handshake:** SYN -> SYN/ACK -> ACK (1.5 RTT). Server keeps half-open
  connections in the **SYN queue**, completed ones in the **accept queue**
  (`listen(fd, backlog)` + `net.core.somaxconn`). A full accept queue = dropped
  SYNs/ACKs -> client retries (latency spikes under load). **SYN flood** defense:
  SYN cookies.
- **Teardown:** FIN/ACK each direction (4 segments). The side that closes
  FIRST enters **TIME_WAIT** for 2xMSL (60 s on Linux) to absorb stray segments
  and ensure the final ACK. Many TIME_WAIT sockets on a client/proxy that opens
  short connections = ephemeral-port exhaustion (~28k ports). Fixes: connection
  pooling/keep-alive, `SO_REUSEADDR` (server rebind), `tcp_tw_reuse` (outgoing),
  let the SERVER close first, widen port range, not `tcp_tw_recycle` (removed).
- **CLOSE_WAIT pile-up** = the application is not calling `close()` after the
  peer closed (a leak bug, not a tuning problem).
- States to know: LISTEN, SYN_SENT, SYN_RECV, ESTABLISHED, FIN_WAIT_1/2,
  CLOSE_WAIT, LAST_ACK, TIME_WAIT. `ss -tan state time-wait | wc -l`.
- **RST** = abort (connection to a closed port, or `SO_LINGER` with 0 timeout);
  a write after the peer closed gets `EPIPE`/SIGPIPE (ignore SIGPIPE or use
  `MSG_NOSIGNAL`).

## Reliability and flow control
- Sequence/ACK numbers, retransmission on timeout (RTO from smoothed RTT) or
  3 duplicate ACKs (**fast retransmit**), SACK, delayed ACKs.
- **Flow control:** receiver advertises a window (`SO_RCVBUF`, window scaling
  lets it exceed 64 KB) so the sender never overruns it. Zero window ->
  persist timer probes. In code: sender sees `EAGAIN` / blocks when send buffer
  is full (`01_socketOptionsAndBuffers.c`).
- **Congestion control** (sender-side, protects the NETWORK): `cwnd` limits
  in-flight data. Slow start (exponential growth to `ssthresh`), congestion
  avoidance (linear), loss -> cut. **Reno/NewReno** halve on loss; **CUBIC**
  (Linux default) grows by a cubic function of time since last loss, good on
  high bandwidth-delay-product links; **BBR** models bottleneck bandwidth and
  min RTT instead of reacting to loss - better on lossy/bufferbloated paths.
  Effective window = min(cwnd, rwnd). Throughput ~ window / RTT; the
  bandwidth-delay product (BDP) says how much must be in flight to fill a pipe.
- **Nagle + delayed ACK:** Nagle holds small writes until the previous data is
  ACKed; the peer delays ACKs ~40 ms -> classic 40 ms latency stall for
  request/response with two small writes. Fix: `TCP_NODELAY`, or coalesce
  writes (`writev`, `MSG_MORE`, `TCP_CORK`).
- `TCP_INFO` (`getsockopt`) exposes rtt, cwnd, retransmits - the first place to
  look for "why is this connection slow" (also `ss -ti`).

## Byte stream and framing (`02_writeAll.c`)
- No message boundaries: loop on partial `read`/`write`; frame with a length
  prefix (validate length against a max!) or delimiters; handle `EINTR`;
  non-blocking: `EAGAIN` -> wait for readiness.
- `read` returning 0 = orderly EOF; `-1` + `ECONNRESET` = RST.
- **Head-of-line blocking:** one lost segment stalls all later bytes on that
  connection (HTTP/2 over TCP suffers it; QUIC/HTTP/3 avoids it with per-stream
  loss recovery over UDP).

## Socket options cheat sheet
| Option | Use |
|---|---|
| `SO_REUSEADDR` | bind a listening port while old connections sit in TIME_WAIT |
| `SO_REUSEPORT` | many sockets on one port, kernel load-balances (per-thread listeners) |
| `TCP_NODELAY` | disable Nagle for latency-sensitive small writes |
| `TCP_CORK` / `MSG_MORE` | batch header+body into full segments |
| `SO_KEEPALIVE` (+ `TCP_KEEPIDLE/INTVL/CNT`) | detect dead peers on idle connections (default 2 h!) |
| `TCP_USER_TIMEOUT` | bound how long unacked data may linger |
| `SO_LINGER` | control close behavior (RST vs graceful) |
| `SO_RCVBUF/SO_SNDBUF` | buffer sizing (setting disables autotuning) |
| `TCP_FASTOPEN` | data in SYN, saves 1 RTT on repeat connections |
| `SO_BUSY_POLL` | spin for low latency |

## Debugging toolbox
`ss -tanpi`, `netstat -s` (retransmits, listen overflows), `tcpdump`/Wireshark
(handshake, retransmissions, zero-window, RST), `iperf3`, `nstat`, `ip -s link`
(drops), `strace -e network`, `/proc/net/tcp`, `sysctl net.ipv4.tcp_*`.

## Senior interviewer Q&A
**Q: Why are there thousands of TIME_WAIT sockets and is that a problem?**
A: The closing side keeps state 60 s so late duplicates can't corrupt a new
connection reusing the 4-tuple. Normal on busy clients/proxies; a problem only
when they exhaust ephemeral ports. Use keep-alive/pooling, make the server
close first, `tcp_tw_reuse`, or more source IPs/ports.

**Q: A request/response service has a mysterious 40 ms latency.**
A: Nagle + delayed ACK on two small writes (header then body). Set
`TCP_NODELAY` or send one buffer (`writev`).

**Q: Explain TCP congestion control in a few sentences.**
A: Sender limits in-flight bytes by `cwnd`: exponential growth in slow start,
linear in congestion avoidance, multiplicative decrease on loss (CUBIC
`beta`=0.7). BBR instead estimates bandwidth and RTT. Flow control (rwnd)
protects the receiver; congestion control (cwnd) protects the network.

**Q: How would you diagnose slow downloads from one region?**
A: Check RTT and loss (`ss -ti`, `mtr`), BDP vs window (scaling disabled,
small `rmem`), retransmits in `tcpdump`, congestion algorithm, middlebox
issues, MTU/PMTU black holes (lower MSS), TLS handshake RTTs, and CDN placement.

**Q: What happens when a client crashes (no FIN) or a cable is pulled?**
A: The server sees nothing until it sends data and retransmits time out
(minutes), or until keepalive probes fail. Use app-level heartbeats,
`SO_KEEPALIVE` tuning, or `TCP_USER_TIMEOUT`.

**Q: `listen` backlog - what happens when it is full?**
A: New SYNs/final ACKs are dropped (`netstat -s | grep -i listen`); clients
retry with exponential backoff, appearing as connect latency. Accept faster,
raise backlog and `somaxconn`, scale accept threads.

**Q: Why UDP/QUIC for some systems?**
A: Avoid head-of-line blocking and ossification, faster handshakes (0/1-RTT),
connection migration, user-space congestion control; trade: you rebuild
reliability, more CPU, firewall friction.

**Q: How do you design a binary protocol over TCP?**
A: Length-prefixed frames with a version/type header, max size limits,
explicit endianness (network order), checksums optional (TCP has one but
weak), request IDs for multiplexing, heartbeats, and graceful close semantics.
