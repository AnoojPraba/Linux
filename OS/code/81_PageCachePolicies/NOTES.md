# Page Cache Replacement Policies

- **LRU:** evict the page unused longest. Needs the list updated on every access (the OS never sees plain memory reads, so kernels approximate it) and is **not scan-resistant**: one big sequential read flushes the working set.
- **O(1) LRU:** hash table (page -> list node) + doubly linked list (front = most recent, tail = victim); delete the hash entry on eviction.
- **Clock (second chance):** a reference bit per page set by the MMU for free; hand sweeps, clears set bits, evicts a clear one. O(1) amortized, nothing on a hit; same scan weakness.
- **2Q / active-inactive lists / ARC / LIRS:** add frequency: new pages enter a probationary FIFO, only a page referenced again is promoted to the protected LRU, so a one-pass scan never reaches it. Linux: active/inactive lists per zone (second reference promotes), MGLRU generations.
- Pattern vs policy: looping access larger than the cache is worst for LRU/FIFO. FIFO has Belady's anomaly (more frames, more faults); LRU/Optimal are stack algorithms and do not.
- Hash tables everywhere in the kernel: dentry cache, inode cache, page cache index, PID lookup; chains short while the table is resized.
