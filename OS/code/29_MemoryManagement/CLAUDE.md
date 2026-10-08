# 29_MemoryManagement

Classic memory-management simulations: FIFO vs LRU page replacement and first-fit vs best-fit contiguous allocation.

## Files
- `01_pageReplacement.c` - fifoPageReplacement and lruPageReplacement with 3 frames over a reference string (prints fault counts)
- `02_contiguousAllocation.c` - firstFit and bestFit placement of processes into memory blocks

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_pageReplacement.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/29_MemoryManagement/` (git-ignored).

## Key concepts / interview angles
- FIFO can show Belady's anomaly (more frames, more faults); LRU does not (stack algorithm).
- The sample reference string is chosen so FIFO has one fewer fault than LRU, to show LRU is not always better.
- OPT (Belady) is the unreachable lower bound; Clock approximates LRU in real kernels.
- First fit is fast; best fit leaves tiny unusable gaps; worst fit and next fit are alternatives.

## Related
- `../30_Paging`
- `../34_SwapThrashingAndWorkingSet`
- `../28_MemoryAddressingAndFragmentation`
- `../../../C_Basics/code/37_LRUCache`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
