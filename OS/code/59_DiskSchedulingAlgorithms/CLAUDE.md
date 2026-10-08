# 59_DiskSchedulingAlgorithms

Disk-head scheduling algorithms (FCFS, SSTF, SCAN, C-SCAN and relatives) on a request queue, with real-world context.

## Files
- `01_diskScheduling.c` - prints the service order and total head movement for FCFS, SSTF, SCAN and C-SCAN (and others) on one request set
- `NOTES.md` - real-world context and a description of the demo

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_diskScheduling.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/59_DiskSchedulingAlgorithms/` (git-ignored).

## Key concepts / interview angles
- SSTF minimises seeks but can starve far requests; SCAN/C-SCAN (elevator) bound waiting; LOOK variants skip the empty ends.
- Metrics: total seek distance, fairness, variance.
- Matters for rotating disks; SSDs and NVMe use simple schedulers (none/mq-deadline/kyber) because there is no seek.
- Linux I/O schedulers: mq-deadline, BFQ, none.

## Related
- `../57_FileSystemStructuresAndAllocation`
- `../60_SpoolingBufferingAndFreeSpace`
- `../25_CPUScheduling`
- `../61_IOManagementPollingInterruptsDMA`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
