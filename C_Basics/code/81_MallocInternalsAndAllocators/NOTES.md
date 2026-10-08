# malloc Internals and Allocator Design

- `malloc` is a user-space library over two kernel interfaces: `brk/sbrk`
  (grow the data segment, one contiguous heap) and `mmap` (anonymous mappings).
  The kernel hands out pages; the allocator carves them into objects.
- **Allocator goals/trade-offs:** speed, low fragmentation, low metadata
  overhead, scalability across threads, locality, security/hardening. You cannot
  max all of them.
- **Fragmentation:** *internal* = rounding/headers waste inside a block (size
  classes, alignment); *external* = enough total free memory but no contiguous
  piece big enough. RSS staying high after `free()` is usually external
  fragmentation or per-thread caches, not a leak.

## Techniques (what to name in an interview)
- **Implicit/explicit free list + first/next/best fit:** simplest; split on
  alloc, coalesce on free. Boundary tags (header + footer) make coalescing with
  the previous block O(1). `01_freelistMalloc.c`.
- **Segregated free lists / size classes:** per-size lists, O(1) alloc/free,
  predictable fragmentation (jemalloc, tcmalloc). `02_slabSizeClasses.c`.
- **Slab/SLUB (kernel):** caches of same-type objects, constructed state
  reused, per-CPU freelists. Great for fixed-size hot objects (`task_struct`,
  `inode`). See `../../../OS/code/46_KernelMemoryAllocatorsAndVirtualization`.
- **Buddy allocator:** power-of-two blocks; split/merge buddies; fast coalescing
  but internal fragmentation (kernel page allocator).
- **Arena/bump/region:** pointer bump, free everything at once. Fastest;
  perfect for request-scoped data. `../../../OS/code/45_CustomAllocator`.
- **Pool/fixed-size:** O(1), zero external fragmentation, ideal for real-time
  and embedded.

## glibc ptmalloc (the default on Linux)
- **Arenas:** one main arena (brk) plus up to 8 x cores secondary arenas
  (mmap'd heaps) to reduce lock contention between threads. Many arenas =
  higher memory footprint in thread-heavy services (`MALLOC_ARENA_MAX`).
- **Chunks:** 16-byte aligned; header holds size and flag bits (in-use of prev,
  mmapped, arena). Freed chunks reuse the payload for `fd/bk` links.
- **Bins:** fastbins (tiny, LIFO, no coalescing), small bins (exact size),
  large bins (sorted ranges), unsorted bin (recently freed staging).
  **tcache** (glibc >= 2.26): per-thread cache of up to 7 chunks per size - lock-free fast path.
- **Large requests:** >= `M_MMAP_THRESHOLD` (128 KB, adjusts dynamically up to
  32 MB) are `mmap`'d and `munmap`'d on free - immediately returned to the OS.
- **Trim:** top-of-heap memory is returned via `brk` only when it is contiguous
  at the top (`M_TRIM_THRESHOLD`); `malloc_trim(0)` also releases free pages
  inside the heap with `madvise`.
- Tools: `mallinfo2`, `malloc_stats()`, `MALLOC_CONF` (jemalloc), `ltrace -e malloc`,
  `valgrind --tool=massif`, heaptrack, `LD_PRELOAD` to swap allocators.

## jemalloc / tcmalloc / mimalloc
- Thread caches + size classes + page-run "extents/spans": mostly lock-free
  fast paths, tight fragmentation control, good profiling (heap profiles).
- Why services swap them in: lower RSS and tail latency with many threads
  (jemalloc in Redis/Firefox/Facebook, tcmalloc in Google). Trade: per-thread
  cache memory, background threads, tuning knobs.

## Bugs allocators surface (and hardening)
- Double free, use-after-free, overflow into the next chunk header (classic
  heap exploitation: tcache poisoning, fastbin dup).
- Mitigations: safe-linking of tcache/fastbin pointers (glibc 2.32), tcache
  double-free key check, `MALLOC_CHECK_`/`MALLOC_PERTURB_`, guard pages
  (electric fence), hardened allocators (scudo), ASan quarantine.
- `malloc(0)` is implementation-defined (NULL or unique pointer);
  `realloc(p, 0)` is a classic trap; `realloc` failure leaves the old block
  allocated - never `p = realloc(p, n)` without a temp; `malloc(n * m)` can
  overflow - use `calloc` / `reallocarray`.
- Alignment: `malloc` returns memory aligned for any fundamental type
  (16 bytes on 64-bit glibc); use `aligned_alloc`/`posix_memalign` for more
  (`../60_AlignedMallocFree`).

## Senior interviewer Q&A
**Q: Design/implement `malloc` and `free`.**
A: Headers with size+free bit, first-fit explicit free list over an arena,
split on alloc, coalesce on free (boundary tags for O(1) both directions),
alignment to 16, grow with `sbrk`/`mmap`. Then discuss: size classes to avoid
search, per-thread caches for scalability, large allocations via mmap, and
hardening. (`01_freelistMalloc.c`.)

**Q: Why might RSS stay high after you free most of your memory?**
A: Fragmentation pins pages (a few live objects per page), top-of-heap not
contiguous-free so brk can't shrink, per-thread caches/arenas retain memory,
or free lists keep it for reuse. Check `malloc_stats`, `/proc/PID/smaps`;
fixes: `malloc_trim`, arena limits, jemalloc/tcmalloc, allocate by lifetime
(arenas), avoid interleaving short- and long-lived objects.

**Q: First fit vs best fit vs size classes?**
A: First fit: fast, tends to fragment the front of the list. Best fit: less
waste but O(n) and leaves tiny slivers. Segregated fit: O(1) with bounded
internal fragmentation; the modern default.

**Q: How does `free(p)` know the size?**
A: It reads the chunk header just before `p` (or looks up the page/span
metadata for `p` in slab-style allocators). That is also why overflowing a
buffer into the next chunk's header corrupts the heap.

**Q: Why are per-thread arenas/caches used and what is the cost?**
A: A single heap lock serializes every malloc; thread-local caches make the
common path lock-free. Cost: memory held by idle threads, cross-thread free
handling (producer allocates, consumer frees), higher worst-case footprint.

**Q: When would you write your own allocator?**
A: Known lifetimes (request/frame arenas), fixed-size hot objects (pools),
real-time/embedded determinism, or avoiding fragmentation in a long-running
daemon - and always measure against jemalloc/tcmalloc first.

**Q: `p = realloc(p, n)` - what is wrong?**
A: On failure `realloc` returns NULL and leaves `p` valid, so the assignment
leaks the original and loses the pointer. Use a temporary. Also `realloc` may
move the block, invalidating other pointers into it.
