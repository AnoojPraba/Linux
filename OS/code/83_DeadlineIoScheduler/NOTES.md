# Deadline I/O Scheduler

- Seek time dominates a spinning disk. An **elevator** (SCAN/LOOK/C-SCAN) keeps requests **sorted by sector** (Linux: red-black tree) and serves in one sweep; adjacent requests are **merged**.
- Pure sector order **starves** far requests. The **deadline** scheduler adds a FIFO per direction ordered by arrival with an expiry (~500 ms read, ~5 s write): normally dispatch sorted **batches**, but if the FIFO head has expired serve it next and continue from there. Reads are preferred (a process waits on them; writes are buffered).
- Same two structures (by position, by arrival) in `mq-deadline`; `BFQ` adds per-process budgets; `kyber` targets latency; for SSD/NVMe `none` (no sort) is often best.
- See also `../59_DiskSchedulingAlgorithms` for FCFS/SSTF/SCAN/C-SCAN/LOOK head-movement totals.
