# 33_VirtualMemoryDeepDive

Virtual memory and the Linux page-fault path, with a demo counting minor faults while touching a 64 MB buffer.

## Files
- `01_pageFaultDemo.c` - allocates 64 MB, touches one byte per 4096-byte stride, reports minor faults via getrusage before and after
- `NOTES.md` - virtual memory deep dive and a step-by-step anatomy of a Linux page fault

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_pageFaultDemo.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/33_VirtualMemoryDeepDive/` (git-ignored).

## Key concepts / interview angles
- Minor fault: page in memory but not mapped (or zero-fill); major fault: needs disk I/O.
- Page-fault path: exception, find VMA, permission check, allocate/map frame (or COW, swap-in, file read), resume.
- Lazy allocation (overcommit) makes malloc succeed without memory; OOM killer on exhaustion.
- Strided touching at the wrong page size gives different fault counts (page size is hard-coded to 4096 in the demo).

## Gotchas
- The demo hard-codes a 4096-byte stride (`PAGE_STRIDE`); this machine's page size is 16384 (`getconf PAGESIZE`), so several touches land in the same page and the minor-fault count is lower than 64 MB / 4 KB.
- Fault counts are machine-specific.

## Related
- `../30_Paging`
- `../34_SwapThrashingAndWorkingSet`
- `../36_CopyOnWrite`
- `../35_MmapFile`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
