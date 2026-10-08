# 31_PageTableEntriesAndTLB

Address translation: splitting virtual addresses into page number and offset, PTE fields, multi-level page tables and the TLB.

## Files
- `01_addressDecomposition.c` - bit arithmetic for (page number, offset) and rebuilding a physical address from a frame number
- `NOTES.md` - PTE contents, single vs multi-level and inverted page tables, TLB, address-translation walkthrough

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_addressDecomposition.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/31_PageTableEntriesAndTLB/` (git-ignored).

## Key concepts / interview angles
- VA = (VPN, offset); PA = (frame number << offset bits) | offset.
- PTE bits: present, writable, user/supervisor, accessed, dirty, NX, frame number.
- Multi-level tables save memory for sparse address spaces; each level adds a memory access on a miss, hence the TLB.
- TLB hit rate and effective access time formula; flush on context switch unless tagged (ASID/PCID).
- Inverted page tables index by frame, not by page.

## Related
- `../30_Paging`
- `../33_VirtualMemoryDeepDive`
- `../27_ContextSwitchMechanics`
- `../32_SegmentationAndOverlays`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
