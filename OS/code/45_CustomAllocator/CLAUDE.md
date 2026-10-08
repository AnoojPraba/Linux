# 45_CustomAllocator

Embedded-style custom allocators: arena (bump) and fixed-size pool with an intrusive free list.

## Files
- `01_arenaAllocator.c` - bump allocator over a static buffer; no per-allocation bookkeeping, reset frees all
- `02_fixedSizePoolAllocator.c` - fixed-size blocks with a free list threaded through the free blocks themselves; supports individual free

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_arenaAllocator.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/45_CustomAllocator/` (git-ignored).

## Key concepts / interview angles
- Arena: O(1) allocate, no individual free, great for per-request lifetimes; alignment must be handled.
- Pool: O(1) alloc/free, no fragmentation for a single size, deterministic (suits RTOS/interrupt context).
- The free list lives inside the free blocks (no extra memory).
- Thread safety needs a lock or per-thread pools.

## Related
- `../../../C_Basics/code/81_MallocInternalsAndAllocators`
- `../../../C_Basics/code/60_AlignedMallocFree`
- `../../../Cpp/code/29_CustomAllocatorCpp`
- `../../../SystemDesign/topics/27_DesignCaseStudyThreadSafeFixedSizeMemoryPool`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
