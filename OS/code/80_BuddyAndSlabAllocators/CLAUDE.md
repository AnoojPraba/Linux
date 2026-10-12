# 80_BuddyAndSlabAllocators

Physical-memory allocation: buddy system, slab object cache, first-fit free list with coalescing and a frame bitmap.

## Files
- `01_buddyAllocator.c` - Free lists per order, split on alloc, coalesce on free using buddy = page ^ (1 << order).
- `02_slabCache.c` - Fixed-size object slots with an intrusive free list (LIFO reuse, no per-object header).
- `03_firstFitFreeList.c` - First fit with split and coalescing of neighbouring holes.
- `04_frameBitmap.c` - One bit per frame: allocate, free, count with bit operations.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_buddyAllocator.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/80_BuddyAndSlabAllocators/` (git-ignored).

## Key concepts / interview angles
- **Buddy:** blocks of 2^k pages, a free list per order. Alloc: take from order k, else split a larger block (upper half goes to the next smaller list). Free: if the **buddy** (`page ^ (1 << order)`, one XOR) is free and same order, merge and repeat upward. Fast, keeps large contiguous blocks (DMA, huge pages); weakness: internal fragmentation (65 pages -> 128) and external fragmentation. Linux: orders 0..10, per-CPU page lists, migration types, `/proc/buddyinfo`.
- **Slab/SLUB:** a cache per object type (`task_struct`, `inode`, `dentry`) carved from buddy pages into equal slots; the free list is **intrusive** (a free slot stores the next pointer inside itself); LIFO reuse = cache-hot objects; per-CPU free arrays avoid locks; constructor reuse; redzones/poisoning (`slub_debug`); `/proc/slabinfo`. A slab is released only when all its objects are free (one long-lived object pins it).
- **First fit / best fit / worst fit:** list of blocks in address order; split on alloc, **coalesce** neighbours on free, otherwise many small useless holes (external fragmentation).
- **Bitmap:** one bit per frame/block (4 GB of 4 KB frames = 128 KB); find first zero (`__builtin_ctzl(~word)`), good for contiguous runs; file systems use it for inode/block allocation. Free list = no extra space but poor at contiguous search.

## Gotchas
- The buddy demo uses a tiny 16-page pool; real allocators track state per page, not by scanning lists.
