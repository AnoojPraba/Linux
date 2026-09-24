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
- See `35_CopyOnWrite` for how `fork()` uses page-table sharing + COW to
  avoid copying a parent's memory eagerly, and `34_MmapFile` for
  file-backed mappings (where a fault can be minor or major depending on
  whether the page is already in the page cache).

## Anatomy of a Linux page fault (step-by-step trace)

1. **MMU translation failure**: the CPU's MMU walks the current page table
   for a virtual address and finds either no valid (present) PTE, or a
   present PTE whose permission bits don't allow the attempted access
   (e.g. writing a read-only page).
2. **CPU raises a page-fault exception**: on x86 this is vector 14. The CPU
   pushes an error code (describing present/not-present, read/write,
   user/kernel, etc.) onto the stack and saves the interrupted execution
   state so the faulting instruction can potentially be resumed later.
3. **Control transfers to the kernel's fault handler**: `do_page_fault()`
   reads the faulting address (`CR2` on x86) and error code, then calls into
   `handle_mm_fault()` to resolve it against the process's memory map.
4. **The handler checks VMAs (virtual memory areas)** covering the faulting
   address:
   - **Legitimate faults**: demand-paged file-backed mapping (page not yet
     read from the file - see `34_MmapFile`), zero-fill anonymous page
     (fresh heap/stack memory - see this folder's `01_pageFaultDemo.c`), or
     a copy-on-write page (see `35_CopyOnWrite`) needing an actual copy on
     write.
   - **Illegitimate faults**: the address isn't backed by any VMA, or the
     access violates the VMA's permissions (e.g. writing a `PROT_READ`
     mapping) - the kernel delivers `SIGSEGV` to the process instead of
     resolving anything.
5. **For a legitimate fault**, the kernel allocates a physical frame (or
   locates the already-cached one for a file-backed page), populates it
   (zero-fills it, copies from the page cache, or copies the COW page), and
   updates the PTE to point at that frame and marks it present (and
   writable, for a resolved COW fault).
6. **The TLB entry for that virtual page is invalidated/updated** (see
   `30_PageTableEntriesAndTLB`) - the old entry (absent, or pointing at the
   pre-COW read-only frame) can't be left cached or the CPU would repeat the
   same fault, or worse, use a stale mapping.
7. **The handler returns and the CPU re-executes the faulting instruction**
   from scratch. This re-execution is what makes page faults transparent to
   the faulting code - the instruction never "sees" the fault; it simply
   appears to have taken slightly longer, now succeeding against the newly
   valid mapping.

`01_pageFaultDemo.c` is the empirical side of this trace: it measures step
5's cost indirectly via `getrusage().ru_minflt`, counting minor faults (steps
1-7 above, but frame population is just a zeroed page - no disk I/O) as it
touches one byte per page of a freshly `malloc()`'d buffer. A major fault
follows the same steps but step 5 additionally blocks on disk I/O (swap-in,
or reading a file-backed page not yet in the page cache), visible instead in
`ru_majflt`.
