# 60_AlignedMallocFree

Implementing aligned malloc/free by hand with the bitmask round-up trick and a stashed original pointer.

## Files
- `01_alignedMalloc.c` - over-allocate, round up with a mask, store the original pointer just before the aligned block, free via it
- `NOTES.md` - why aligned allocations are needed (SIMD, DMA, cache lines), the bitmask trick, why the original pointer must be stashed

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_alignedMalloc.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/60_AlignedMallocFree/` (git-ignored).

## Key concepts / interview angles
- Round up: `(p + a - 1) & ~(a - 1)`; requires alignment `a` to be a power of two.
- Over-allocate by `a - 1 + sizeof(void*)`; keep the raw pointer so `free` gets what malloc returned.
- Standard options: `posix_memalign`, `aligned_alloc` (C11, size multiple of alignment).
- Uses: SIMD loads, DMA buffers, cache-line-aligned structures.

## Related
- `../59_MemoryAlignmentAndPadding`
- `../81_MallocInternalsAndAllocators`
- `../../../OS/code/45_CustomAllocator`
- `../05_BitManipulation/06_isPowerOfTwo.c`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
