# Virtual Memory Deep Dive

- Every process sees a private virtual address space; the MMU translates
  virtual addresses to physical addresses using per-process page tables.
- **Page table**: multi-level tree of page table entries (PTEs), each
  mapping a virtual page to a physical frame (plus permission/present bits).
  Multi-level (not a flat array) keeps the table sparse and small for
  processes that only use a fraction of their address space.
- **TLB (Translation Lookaside Buffer)**: a small hardware cache of recent
  virtual->physical translations. A TLB hit skips the page-table walk
  entirely; a TLB miss requires walking the page table (slow) then caching
  the result. Context switches typically flush/partition the TLB (or use
  address-space IDs), one reason context switches are expensive.
- **Page fault**: a trap raised when a virtual address has no valid mapping
  (or the mapping doesn't allow the attempted access). Two kinds:
  - **Minor fault**: the page exists in memory already (e.g. a fresh
    zero-filled anonymous page, or a page shared via copy-on-write) but
    isn't yet mapped into this process's page table - just needs a mapping,
    no disk I/O.
  - **Major fault**: the page's data must be fetched from disk (swap, or a
    memory-mapped file) - orders of magnitude slower.
- **Demand paging**: `malloc()`/`mmap()` reserve virtual address space
  immediately but do not allocate physical frames until the memory is first
  touched (written or read) - each first touch is a page fault. This demo's
  `getrusage().ru_minflt` count rises roughly once per page touched.
- See `60_CopyOnWrite` for how `fork()` uses page-table sharing + COW to
  avoid copying a parent's memory eagerly, and `59_MmapFile` for
  file-backed mappings (where a fault can be minor or major depending on
  whether the page is already in the page cache).
