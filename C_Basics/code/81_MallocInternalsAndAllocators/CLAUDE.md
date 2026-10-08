# 81_MallocInternalsAndAllocators

How malloc works: a free-list allocator with split/coalesce, a size-class (slab) allocator, and observing glibc brk vs mmap behaviour, with a senior Q&A.

## Files
- `01_freelistMalloc.c` - explicit free list over a fixed arena: first fit, block splitting, coalescing on free, per-block header
- `02_slabSizeClasses.c` - segregated-fit size-class allocator (core idea of jemalloc/tcmalloc/slab)
- `03_brkVsMmapAndRss.c` - prints RSS and brk through small, 1MB, 100k x 200B allocations, frees and malloc_trim (reads /proc/self/statm)
- `NOTES.md` - techniques to name in an interview, glibc ptmalloc, jemalloc/tcmalloc/mimalloc, hardening, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_freelistMalloc.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/81_MallocInternalsAndAllocators/` (git-ignored).

## Key concepts / interview angles
- Block header stores size/flags; `free(p)` finds the size from the header before p.
- First fit vs best fit vs size classes: fragmentation (external vs internal) vs speed.
- Coalescing adjacent free blocks fights external fragmentation; boundary tags allow O(1) backward merge.
- glibc: small requests grow brk, >= M_MMAP_THRESHOLD (128KB default, dynamic) use mmap and return to the OS on free; freed holes can pin RSS.
- Per-thread arenas/caches cut lock contention at the cost of memory.
- `p = realloc(p, n)` leaks on failure.

## Gotchas
- `03_brkVsMmapAndRss.c` is Linux/glibc specific (/proc/self/statm, sbrk, `mallinfo2` needs glibc 2.33+); numbers vary by system.

## Related
- `../60_AlignedMallocFree`
- `../15_DynamicMemory`
- `../../../OS/code/45_CustomAllocator`
- `../../../OS/code/46_KernelMemoryAllocatorsAndVirtualization`
- `../../../Cpp/code/29_CustomAllocatorCpp`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
