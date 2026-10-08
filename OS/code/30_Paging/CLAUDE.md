# 30_Paging

Paging basics seen from user space: querying the page size, anonymous mmap and observing RSS growth under demand paging.

## Files
- `01_pageSizeAndMmap.c` - sysconf page size queried at runtime; anonymous MAP_PRIVATE mapping
- `02_demandPaging.c` - reads VmRSS from /proc/self/status before and after touching mapped pages

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_pageSizeAndMmap.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/30_Paging/` (git-ignored).

## Key concepts / interview angles
- Pages are fixed-size units (4 KB typical on x86-64; `getconf PAGESIZE` reports 16384 on this Raspberry Pi, so always query it).
- Demand paging: mmap/malloc reserves virtual space; physical frames are allocated on first touch.
- RSS vs virtual size; anonymous pages are zero-filled on first fault.
- Huge pages reduce TLB pressure.

## Gotchas
- Output (page size, RSS values) is machine-specific.

## Related
- `../31_PageTableEntriesAndTLB`
- `../33_VirtualMemoryDeepDive`
- `../35_MmapFile`
- `../29_MemoryManagement`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
