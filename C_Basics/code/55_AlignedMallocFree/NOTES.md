# Aligned malloc/free (hand-rolled)

`posix_memalign`/`aligned_alloc` already solve this in the standard library
- the point of this folder is to understand and implement the underlying
technique by hand, since it comes up both as an interview question and when
targeting platforms/allocators without those functions.

## Why you'd need aligned allocations

- **SIMD instructions**: many vectorized load/store instructions (SSE
  requires 16-byte alignment, AVX 32-byte) fault or are slower on misaligned
  addresses.
- **Cache-line alignment**: aligning a buffer to 64 bytes (a common cache
  line size) avoids false sharing between unrelated data that would
  otherwise land on the same cache line - see
  `../../OS/code/38_FalseSharing` for a benchmark of the performance impact
  false sharing causes.
- **Custom allocators / hardware requirements**: some DMA engines or memory-
  mapped hardware regions require specific alignment.

## The bitmask alignment trick

Given a `rawAddr` and a power-of-two `alignment`:

```c
alignedAddr = (rawAddr + alignment - 1) & ~(alignment - 1);
```

- `alignment - 1` is a mask of all 1-bits below the alignment boundary (e.g.
  for alignment 64, `0x3F` = `00111111`).
- `~(alignment - 1)` flips that to all 1-bits *above* the boundary and 0s
  below it (e.g. `0xFFFFFFC0`), which zeroes out any address bits that would
  make the address not a multiple of `alignment`.
- Adding `alignment - 1` first ensures that if `rawAddr` is already
  misaligned, the subsequent masking rounds it *up* to the next aligned
  address rather than down to a previous (too-small) one.
- This only works correctly when `alignment` is a power of two, since only
  then does `alignment - 1` produce a clean run of low-order 1-bits.
  `isPowerOfTwo()` (`(value & (value - 1)) == 0`) verifies this before the
  arithmetic is trusted.

## Why you must stash the original pointer

- `malloc()` returns some raw address; the aligned address returned to the
  caller is computed by rounding that address *up*, so it is almost always a
  **different** address than what `malloc()` gave you.
- Calling `free()` directly on the aligned address would be undefined
  behavior - `free()` requires exactly the pointer `malloc()` (or friends)
  returned.
- The fix: reserve `sizeof(void *)` bytes immediately before the aligned
  address and store the original raw pointer there. `aligned_free()` walks
  back by `sizeof(void *)`, reads that stored pointer, and frees it instead.
