# Segmentation and Overlays

Notes-only: segmentation is a memory-model/hardware-addressing concept, not
something meaningfully demoable in userspace C on a modern paged x86_64
Linux system (the MMU here is paging-only from userspace's point of view) -
a contrived demo would just simulate arithmetic already shown by the
addressing notes in `27_MemoryAddressingAndFragmentation`.

- **Segmentation**: divides a process's logical address space into
  variable-sized, meaningful units - code segment, data segment, stack
  segment, heap segment - each with its own base+limit register pair. An
  address is a `(segment, offset)` pair; the MMU adds `offset` to that
  segment's base and checks it against the segment's limit. Unlike paging's
  fixed-size frames, segment sizes match the logical unit they represent,
  which maps naturally onto how programmers/compilers already think about a
  program's structure (this is why segmentation is described as matching
  the *logical* view of memory, vs paging's purely mechanical fixed slices).
- **Segmentation vs paging**: segmentation gives natural logical units but
  reintroduces external fragmentation (variable-sized segments leave
  variable-sized holes, same problem as contiguous allocation in
  `27_MemoryAddressingAndFragmentation`). Paging (`29_Paging`,
  `32_VirtualMemoryDeepDive`) uses fixed-size frames, eliminating external
  fragmentation at the cost of some internal fragmentation and losing the
  logical-unit boundary. Neither is strictly better - it's a real trade-off.
- **Segmentation with paging (hybrid)**: x86 protected mode supports both at
  once - a logical address is first translated `(segment, offset)` ->
  linear address via segment base+limit, then the linear address is run
  through the normal paged translation to get a physical address. Modern
  x86-64 OSes (Linux included) effectively disable meaningful segmentation
  (flat segments with base 0, limit covering the whole address space) and
  rely on paging alone for protection/translation - segment registers
  mostly survive today for things like thread-local storage (`fs`/`gs`
  base) rather than address-space partitioning.
- **Segmentation fault ("segfault")**: the term comes directly from this
  model - a **segmentation fault** is the hardware trap raised when an
  access falls outside a segment's valid base+limit range (or, on a purely
  paged system like modern Linux, when it hits a virtual address with no
  valid page-table mapping or one that disallows the attempted operation,
  e.g. writing to a read-only page). The name stuck even though most
  "segfaults" today are really page-table/permission faults, not literal
  segment-limit violations - see `32_VirtualMemoryDeepDive`'s page-fault
  notes for the paging-side mechanics.
- **Overlays**: a pre-virtual-memory technique for running a program larger
  than physical memory on systems with no demand paging. The programmer (or
  a linker-assisted overlay manager) manually splits the program into
  mutually-exclusive chunks ("overlays") that share the same memory region,
  loading whichever overlay is needed and discarding/overwriting the
  previous one - e.g. a game's "load level 2 code over level 1's now-unused
  code". This required careful manual management of what code/data could
  safely coexist and painful bookkeeping of overlay boundaries by hand.
- **Why demand-paged virtual memory made overlays obsolete**: demand paging
  (`29_Paging`'s `02_demandPaging.c`) gives every process the illusion of a
  full private address space, automatically keeping only the actively-used
  pages resident and transparently paging the rest in/out on fault - the
  OS/MMU do automatically and per-page what overlays required a programmer
  to do manually and per-chunk. Once virtual memory with demand paging
  became standard, hand-rolled overlays were no longer needed for ordinary
  programs (they still show up occasionally in extremely memory-constrained
  environments without an MMU, e.g. some embedded/bootloader code).
