# 28_MemoryAddressingAndFragmentation

Contiguous allocation strategies (next fit) and the buddy system, plus logical vs physical addressing and internal/external fragmentation.

## Files
- `01_nextFit.c` - next-fit allocation resuming from the last allocation point
- `02_buddySystem.c` - power-of-two buddy allocator with per-order free/used state and merging
- `NOTES.md` - logical vs physical addresses, contiguous allocation, internal vs external fragmentation, how the demos differ from first/best fit

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_nextFit.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/28_MemoryAddressingAndFragmentation/` (git-ignored).

## Key concepts / interview angles
- Internal fragmentation: waste inside an allocated block; external: free memory split into unusable pieces.
- Next fit avoids rescanning fragmented low blocks but can fragment the tail.
- Buddy system: split by powers of two, merge buddies by XORing the address with the block size; fast merge, internal fragmentation cost; used by the Linux page allocator.
- Compaction and paging remove external fragmentation.

## Related
- `../29_MemoryManagement`
- `../30_Paging`
- `../46_KernelMemoryAllocatorsAndVirtualization`
- `../../../C_Basics/code/81_MallocInternalsAndAllocators`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
