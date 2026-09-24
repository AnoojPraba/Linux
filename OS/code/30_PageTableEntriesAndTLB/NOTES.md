# Page Table Entries and TLB

- **Page Table Entry (PTE)** - per-page-table-row metadata the MMU consults on every
  translation. Typical bits:
  - **Valid/present bit**: is this mapping currently active? If clear, any access
    traps as a page fault (see `32_VirtualMemoryDeepDive`'s page-fault-demo folder).
  - **Physical frame number**: the high bits of the physical address this virtual
    page maps to.
  - **Protection bits**: read/write/execute permissions - e.g. the W^X (write xor
    execute) policy enforced by marking code pages read+execute and data pages
    read+write but never both.
  - **Dirty bit**: set by hardware when the page has been written since it was
    loaded - tells the OS whether a page being evicted must be written back (to
    swap or its backing file) or can simply be dropped.
  - **Accessed/reference bit**: set by hardware on any access (read or write) -
    this is exactly what the Second-Chance/Clock replacement algorithm in
    `33_SwapThrashingAndWorkingSet/02_clockPageReplacement.c` reads and clears as
    its hand sweeps frames.
  - Other flags: cache-disable (for MMIO/device memory that must not be cached),
    global (pins a mapping - e.g. kernel space - across TLB flushes/context
    switches), user/supervisor (is this page accessible from user mode).

- **Single-level vs multi-level page tables**:
  - A single-level (flat array) page table needs one PTE per possible virtual
    page in the entire address space, whether or not that page is ever used.
    For a large address space (e.g. 48-bit virtual addresses) this table would
    be enormous - and most processes only actually use a small, sparse subset
    of their address space (a bit of heap, a bit of stack, a bit of code), so
    nearly all of that table would be wasted, unused entries.
  - Multi-level (hierarchical) page tables fix this by only allocating the
    lower-level tables that are actually needed. x86-64 uses 4 levels: PML4 ->
    PDPT (Page Directory Pointer Table) -> PD (Page Directory) -> PT (Page
    Table), each level indexed by a slice of the virtual address's bits. If a
    whole PML4 or PDPT entry's subtree is unused, that entire branch is never
    allocated at all.
  - Cost: more memory references per translation (up to 4 sequential loads for
    a 4-level walk on a TLB miss) versus 1 for a flat table - a classic
    time/space tradeoff, and exactly why the TLB exists (below) to hide that
    cost on the common case.

- **Inverted page tables**: instead of one entry per virtual page (per process),
  keep one entry per *physical frame*, indexed by frame number, storing which
  (process, virtual page) currently owns that frame. Size is proportional to
  physical memory, not virtual address space, so it's dramatically smaller for
  systems with a huge virtual address space but modest physical RAM. Tradeoff:
  lookup is no longer a direct index by virtual page number - translating a
  virtual address now requires a hash table or associative search over the
  inverted table to find which frame (if any) holds that virtual page.

- **TLB (Translation Lookaside Buffer)**: a small, fast, fully-associative (or
  set-associative) hardware cache sitting between the CPU and the page-table
  walk, caching recent virtual-page -> physical-frame translations so most
  memory accesses skip the (possibly multi-level) walk entirely.
  - **TLB hit**: translation found in the TLB - address ready in ~1 cycle.
  - **TLB miss**: translation not cached - triggers a page-table walk. On some
    architectures (x86) this walk is done by dedicated hardware ("hardware
    page-table walker"); on others (classic MIPS, some RISC designs) a TLB miss
    traps to software and the OS's exception handler walks the table itself.
  - **TLB flush**: invalidating some/all TLB entries. Required on a context
    switch to a *different* address space, since cached translations belong to
    the outgoing process's page tables and would silently mistranslate for the
    incoming one. Unnecessary flushes hurt because every flushed entry becomes
    a guaranteed TLB miss (-> full page-table walk) the next time that page is
    touched - one of the hidden costs of a context switch called out in
    `26_ContextSwitchMechanics`.
  - **Tagged TLBs / ASIDs (PCID on x86)**: tag each TLB entry with an
    address-space ID so entries from multiple processes can coexist in the TLB
    without a full flush on every switch - only entries for the ID being
    switched *away from* would ever need invalidating, and typically not even
    those, since they simply become inactive until that process runs again.

- **Address translation walkthrough**:
  1. CPU issues a virtual address; hardware splits it into a **page number**
     (high bits) and a **page offset** (low bits, sized to the page - e.g. the
     low 12 bits for a 4 KB page).
  2. The page number is first checked against the TLB. On a hit, the frame
     number comes straight from the TLB entry.
  3. On a miss, the page number indexes into the (possibly multi-level) page
     table - one slice of bits per level - to find the PTE and its frame
     number, then that translation is cached into the TLB for next time.
  4. If the PTE's valid bit is clear (or the access violates its protection
     bits), this is a page fault, not a successful translation - this is
     exactly the "TLB miss + page-table walk finds an invalid PTE" path that
     `32_VirtualMemoryDeepDive`'s `01_pageFaultDemo.c` demonstrates from
     userspace via `getrusage().ru_minflt`/`ru_majflt`.
  5. Otherwise, the frame number is combined with the unchanged page offset to
     produce the final physical address: `physical = (frame_number << offset_bits)
     | offset`.
