# 79_VmaAndMultiLevelPageTable

Finding the memory region of an address (VMAs: lookup, merge, split) and a multi-level page table as a radix tree.

## Files
- `01_vmaLookupMergeSplit.c` - Sorted VMA array with binary search; adjacent mappings with equal permissions merge; munmap/mprotect in the middle splits.
- `02_multiLevelPageTableTrie.c` - 3 levels of 9 bits: tables allocated only on demand, translate walks the trie, prints tables used vs a flat table.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_vmaLookupMergeSplit.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/79_VmaAndMultiLevelPageTable/` (git-ignored).

## Key concepts / interview angles
- A process address space is a set of non-overlapping **VMAs** (start, end, permissions, backing). Page fault: find the VMA (none -> SIGSEGV), check permission against the access (write to `r--` -> SIGSEGV), then allocate/load the page.
- Needed operations: find VMA containing an address O(log n) (Linux: red-black tree + list, now a **maple tree**), find a free gap for `mmap`, **merge** a new mapping with neighbours of equal permissions/backing, **split** on `munmap`/`mprotect` in the middle (a guard page inside a big mapping adds two VMAs). `vm.max_map_count` (65530) is hit by many tiny differing mappings -> `mmap` `ENOMEM`.
- A flat table for 48-bit addresses/4 KB pages = 2^36 entries = 512 GB per process. A **multi-level page table is a radix tree (trie)**: x86-64 uses four 9-bit indexes (PGD, PUD, PMD, PTE), each table 4 KB/512 entries, **allocated only when something below is mapped**. A walk = 4 memory accesses, hence the **TLB** caches full translations; huge pages stop one level early. 5-level paging adds 57-bit addresses; inverted page tables scale with physical memory.
- Same radix-tree idea: page cache index, IP routing tables, file extent maps.

## Gotchas
- munmap of a range spanning several VMAs needs a loop; the demo handles a range inside one VMA.
