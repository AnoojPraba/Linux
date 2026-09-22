# Network Stack Basics

Notes-focused: this repo already has runnable socket demos in
`69_SocketProgramming` (TCP client/server), `70_UDPSockets` (UDP client/
server), and multiplexing demos in `71_IOMultiplexing` (select/poll/epoll) -
this folder does not re-demo that code, it covers the conceptual layer above
it that interviewers ask about directly.

## TCP three-way handshake / teardown

- **Handshake (connection setup)**:
  1. Client sends `SYN` (seq = x).
  2. Server responds `SYN-ACK` (seq = y, ack = x+1).
  3. Client sends `ACK` (ack = y+1). Connection is now `ESTABLISHED`.
- **Teardown (four-way, normally)**:
  1. Side A sends `FIN`.
  2. Side B sends `ACK` for A's FIN.
  3. Side B sends its own `FIN` when done.
  4. Side A sends `ACK` for B's FIN.
  The side that sent the first `FIN` and got the final `ACK` enters
  `TIME_WAIT` for `2*MSL` (maximum segment lifetime) before fully closing,
  to absorb any delayed duplicate packets from the old connection.
- `SO_REUSEADDR` lets a new socket bind to a port stuck in `TIME_WAIT`.

## Congestion control (conceptual)

- Goal: avoid overwhelming the network path, distinct from flow control
  (which protects the *receiver* from being overwhelmed, via the advertised
  window).
- **Slow start**: congestion window (`cwnd`) starts small and doubles each
  RTT (exponential growth) until it hits a threshold or a loss is detected.
- **Congestion avoidance**: after slow start's threshold, `cwnd` grows
  linearly (roughly +1 segment per RTT) instead of exponentially.
- **Loss response**: classic Reno-style TCP halves `cwnd` on a detected loss
  (multiplicative decrease) - "additive increase, multiplicative decrease"
  (AIMD) is the core fairness-inducing pattern. Modern variants (CUBIC,
  BBR) use more sophisticated signals (queuing delay, bandwidth estimates)
  instead of relying purely on loss.

## epoll vs select/poll internals

- **`select()`**: caller passes a bitmask (fd_set) of fds to watch; kernel
  scans it, and returns a mutated copy - caller must rebuild the fd_set
  and re-scan every returned fd each call. Fixed max fd count (`FD_SETSIZE`,
  often 1024). O(n) per call in both userspace and kernel.
  See `71_IOMultiplexing/01_selectMultiplePipes.c`.
- **`poll()`**: like select but uses a resizable array of `struct pollfd`
  instead of a bitmask - no `FD_SETSIZE` limit, but still O(n) scan per call
  in the kernel and requires re-passing the whole array each time.
  See `71_IOMultiplexing/02_pollMultiplePipes.c`.
- **`epoll()`**: kernel keeps a persistent interest list (`epoll_create()` +
  `epoll_ctl()` to add/remove/modify fds once) instead of re-scanning
  everything each call. `epoll_wait()` only returns the fds that actually
  became ready, typically via a ready-list maintained by callbacks from the
  device drivers - O(1) amortized per ready event instead of O(n) over all
  watched fds, which is why epoll scales far better with large fd counts
  (the classic C10K problem). See `71_IOMultiplexing/03_epollServer.c`.
- Edge-triggered (`EPOLLET`) vs level-triggered epoll: level-triggered (the
  default, like select/poll) keeps reporting a fd as ready until it's fully
  drained; edge-triggered reports only on a state transition, requiring the
  caller to read until `EAGAIN` each time to avoid missing data.
