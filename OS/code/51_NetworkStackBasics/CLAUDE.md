# 51_NetworkStackBasics

Conceptual notes on the TCP handshake and teardown, congestion control and epoll vs select/poll internals (NOTES-only).

## Files
- `NOTES.md` - three-way handshake and teardown, congestion control (conceptual), epoll vs select/poll internals

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- SYN, SYN-ACK, ACK; four-way close with TIME_WAIT on the active closer (why `SO_REUSEADDR` matters).
- Congestion control: slow start, congestion avoidance, fast retransmit/recovery; cwnd vs rwnd.
- select/poll are O(n) per call and copy fd sets; epoll keeps an interest list in the kernel and returns only ready fds.
- Layers: socket buffers, TCP/IP stack, qdisc, NIC (interrupts, NAPI).

## Related
- `../52_SocketProgramming`
- `../54_IOMultiplexing`
- `../67_EpollInDepth`
- `../69_TcpDeepDive`
- `../../../SystemDesign/topics/11_DNSAndTLSBasics`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
