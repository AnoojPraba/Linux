# Memory Addressing and Fragmentation

- **Logical/virtual vs physical address**: a program only ever deals in
  logical (virtual) addresses - offsets into its own private address space.
  The **MMU** (memory management unit) translates each logical address to a
  physical RAM address at access time (via the page table on paged systems,
  or base+limit on segmented ones) - the CPU never issues a raw physical
  address itself. This indirection is what lets the OS relocate a process in
  physical memory, swap parts of it out, or give it more space than
  physically exists, all without the program's code changing.
- **Contiguous memory allocation**: an older model (pre-paging) where each
  process is given one unbroken range of physical memory, tracked via a
  base+limit pair. Simple and fast to translate (one add, one bounds check),
  but the whole process must fit in one contiguous hole, which is the root
  cause of external fragmentation below. See `28_MemoryManagement` for
  first-fit/best-fit demos of choosing which hole to use.
- **Internal fragmentation**: wasted space *inside* an allocated block,
  because the allocator only hands out fixed-size chunks (e.g. a buddy
  system rounding a 130-byte request up to a 256-byte block wastes 126
  bytes inside that block). The memory is allocated but unusable by anyone
  else.
- **External fragmentation**: wasted space *between* allocated blocks - many
  small free holes scattered across memory, none individually large enough
  to satisfy a new request, even though their sum would be. Classic
  first-fit/best-fit contiguous allocation suffers from this over time.
  Compaction (sliding allocations together) or switching to paging (which
  allocates in fixed frame-sized units, sidestepping the need for
  contiguity) are the usual fixes.
- **This folder's demos** show two allocation *strategies* distinct from the
  first-fit/best-fit already covered in `28_MemoryManagement`:
  - **Next-fit**: like first-fit, but resumes scanning from where the last
    search left off instead of always restarting at the beginning - avoids
    re-scanning already-fragmented low memory on every request, at the cost
    of fragmenting the *whole* block list evenly instead of concentrating
    fragmentation near the start.
  - **Buddy system**: a power-of-2 splitting scheme that allocates from a
    single large arena, recursively halving a free block until it's the
    smallest power of 2 that still fits the request, and recursively
    coalescing two adjacent "buddy" blocks back together when both become
    free. Bounds external fragmentation (every free block is a power-of-2
    size, so blocks combine cleanly) at the cost of internal fragmentation
    (rounding up to the next power of 2).
- Contrast with `42_CustomAllocator`, which covers pool/arena allocators -
  allocation *layout* strategies for a language-level `malloc()` replacement
  (bump-pointer arenas, fixed-size free-list pools) - whereas next-fit and
  buddy here are about how the OS/allocator *chooses which free block* to
  hand out and how it *manages fragmentation* of the underlying memory.
