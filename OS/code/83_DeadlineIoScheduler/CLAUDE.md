# 83_DeadlineIoScheduler

Elevator plus deadline I/O scheduling: sorted sweep with FIFO expiry so far requests are not starved.

## Files
- `01_deadlineScheduler.c` - Sorted-sector sweep with an arrival-order deadline; the far request is pulled forward at the cost of extra head movement.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_deadlineScheduler.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/83_DeadlineIoScheduler/` (git-ignored).

## Key concepts / interview angles
- Seek time dominates a spinning disk. An **elevator** (SCAN/LOOK/C-SCAN) keeps requests **sorted by sector** (Linux: red-black tree) and serves in one sweep; adjacent requests are **merged**.
- Pure sector order **starves** far requests. The **deadline** scheduler adds a FIFO per direction ordered by arrival with an expiry (~500 ms read, ~5 s write): normally dispatch sorted **batches**, but if the FIFO head has expired serve it next and continue from there. Reads are preferred (a process waits on them; writes are buffered).
- Same two structures (by position, by arrival) in `mq-deadline`; `BFQ` adds per-process budgets; `kyber` targets latency; for SSD/NVMe `none` (no sort) is often best.
- See also `../59_DiskSchedulingAlgorithms` for FCFS/SSTF/SCAN/C-SCAN/LOOK head-movement totals.

## Gotchas
- Deadline logic trades extra head movement for bounded wait (the demo prints both).
