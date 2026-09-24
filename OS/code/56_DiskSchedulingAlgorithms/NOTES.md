# Disk Scheduling Algorithms

Given a disk head at some starting cylinder and a queue of pending requested
cylinders, these algorithms decide the SERVICE ORDER, trading off total head
movement (seek distance, i.e. throughput) against fairness / worst-case wait
per request.

- **FCFS (First-Come-First-Served)**: service requests strictly in arrival
  order. Simple and fair (no starvation), but ignores locality entirely -
  the head can swing wildly back and forth, giving poor total seek distance
  under a busy, scattered request stream.
- **SSTF (Shortest Seek Time First)**: always service whichever pending
  request is closest to the current head position. Minimizes seek time
  greedily and generally beats FCFS on total movement, but can starve
  requests that are far from the "hot" region the head keeps servicing near
  - not fair, and starvation is a real risk under continuous load.
- **SCAN (elevator algorithm)**: head sweeps in one direction (e.g.
  increasing cylinder number), servicing every pending request it passes,
  until it reaches the end of the disk, then reverses direction and sweeps
  back. Bounds the worst-case wait (no request waits more than one full
  sweep) and gives good throughput, at the cost of the two ends of the disk
  getting serviced less frequently than the middle (the middle gets swept
  twice as often).
- **C-SCAN (Circular SCAN)**: like SCAN, but only services requests while
  sweeping in one direction; on reaching the end, jumps back to the
  beginning without servicing anything on the return trip, then sweeps
  forward again. Treats the disk as circular, giving a more UNIFORM wait
  time across all cylinders than SCAN (no cylinder is serviced twice as
  often as another), at the cost of the "wasted" return jump not doing any
  useful work.

## Real-world context
These are the textbook ancestors of real Linux I/O schedulers: the
historical "elevator" scheduler was literally SCAN-like; later schedulers
(`deadline`, `cfq` - Completely Fair Queuing, `mq-deadline`, `BFQ` - Budget
Fair Queuing) layer in per-request deadlines and fairness between processes
on top of the same basic seek-minimization idea.

On **SSDs**, there is no physical disk arm/seek time - any block is roughly
equal-cost to access (electronically addressed, no moving parts) - so these
seek-minimizing algorithms bring little/no benefit there. Modern schedulers
detect non-rotational (SSD/NVMe) devices and use simpler queuing policies
(or bypass scheduling largely, e.g. `none`/`noop`-style) tuned for I/O depth
and command queuing instead of head movement.

## Demo (`01_diskScheduling.c`)
Runs the same disk size, head start position, and request sequence through
FCFS, SSTF, SCAN, and C-SCAN, printing the service order and total head
movement (seek distance) for each - a concrete way to see SSTF/SCAN beating
FCFS, and C-SCAN's more uniform sweep behavior vs SCAN.
