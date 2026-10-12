# 81_PageCachePolicies

LRU with hash + doubly linked list, LRU versus 2Q on a scan, and a name-cache hash table.

## Files
- `01_lruVs2qScanResistance.c` - Replay a trace with a hot set and a long sequential scan; LRU loses the hot set, 2Q keeps it.
- `02_lruHashAndList.c` - O(1) LRU: hash table to find the node, doubly linked list for recency, evict the tail.
- `03_nameCacheHashTable.c` - dentry-cache style chained hash table with hit/miss counters.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_lruVs2qScanResistance.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/81_PageCachePolicies/` (git-ignored).

## Key concepts / interview angles
- **LRU:** evict the page unused longest. Needs the list updated on every access (the OS never sees plain memory reads, so kernels approximate it) and is **not scan-resistant**: one big sequential read flushes the working set.
- **O(1) LRU:** hash table (page -> list node) + doubly linked list (front = most recent, tail = victim); delete the hash entry on eviction.
- **Clock (second chance):** a reference bit per page set by the MMU for free; hand sweeps, clears set bits, evicts a clear one. O(1) amortized, nothing on a hit; same scan weakness.
- **2Q / active-inactive lists / ARC / LIRS:** add frequency: new pages enter a probationary FIFO, only a page referenced again is promoted to the protected LRU, so a one-pass scan never reaches it. Linux: active/inactive lists per zone (second reference promotes), MGLRU generations.
- Pattern vs policy: looping access larger than the cache is worst for LRU/FIFO. FIFO has Belady's anomaly (more frames, more faults); LRU/Optimal are stack algorithms and do not.

## Gotchas
- 2Q needs probation capacity larger than the hot set to promote anything; with a too-small probation FIFO nothing is promoted (try A1_CAP = 2).
