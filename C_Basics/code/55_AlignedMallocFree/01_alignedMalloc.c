#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define CACHE_LINE_ALIGNMENT 64
#define SIMD_ALIGNMENT 16
#define DEMO_BUFFER_BYTES 100

// Hand-rolled aligned malloc/free using only standard malloc/free (i.e. not
// posix_memalign/aligned_alloc, which already solve this problem - the point
// here is to implement the underlying technique). See NOTES.md for the
// bitmask trick and why aligned_free cannot just call free() on the
// returned pointer.

/*****************************************************************************
 * Name: isPowerOfTwo
 *
 * Description:
 *         Checks whether a size_t value is a power of two.
 *
 * Inputs:
 *         value : the value to check.
 *
 * Returns:
 *         1 if value is a nonzero power of two, 0 otherwise.
 *****************************************************************************/
int isPowerOfTwo(size_t value)
{
    return (value != 0) && ((value & (value - 1)) == 0);
}

/*****************************************************************************
 * Name: aligned_malloc
 *
 * Description:
 *         Allocates a block of memory whose address is a multiple of
 *         alignment, using only standard malloc(). The original malloc'd
 *         pointer is stashed just before the returned aligned address so
 *         aligned_free() can recover and free it.
 *
 * Inputs:
 *         bytes     : number of usable bytes requested.
 *         alignment : required alignment, must be a power of two.
 *
 * Returns:
 *         Pointer to an aligned block of at least bytes usable bytes, or
 *         NULL on invalid alignment or allocation failure.
 *****************************************************************************/
void *aligned_malloc(size_t bytes, size_t alignment)
{
    void *raw;
    uintptr_t rawAddr;
    uintptr_t alignedAddr;
    void **storedPtrSlot;

    assert(isPowerOfTwo(alignment));
    if (!isPowerOfTwo(alignment))
    {
        return NULL;
    }

    // Worst case, the raw block starts 1 byte past an alignment boundary, so
    // we need up to (alignment - 1) extra bytes to reach the next aligned
    // address, plus room to stash the original pointer before it.
    raw = malloc(bytes + alignment - 1 + sizeof(void *));
    if (raw == NULL)
    {
        return NULL;
    }

    rawAddr = (uintptr_t) raw + sizeof(void *);
    // Round up to the next multiple of alignment via the bitmask trick (see
    // NOTES.md): valid only because alignment is a verified power of two.
    alignedAddr = (rawAddr + alignment - 1) & ~(alignment - 1);

    storedPtrSlot = (void **) (alignedAddr - sizeof(void *));
    *storedPtrSlot = raw;

    return (void *) alignedAddr;
}

/*****************************************************************************
 * Name: aligned_free
 *
 * Description:
 *         Frees a block previously returned by aligned_malloc(), recovering
 *         the original malloc'd pointer stashed just before it.
 *
 * Inputs:
 *         ptr : pointer previously returned by aligned_malloc(), or NULL.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void aligned_free(void *ptr)
{
    void **storedPtrSlot;
    void *raw;

    if (ptr == NULL)
    {
        return;
    }

    // The aligned address is NOT what malloc gave us - calling free()
    // directly on it would be undefined behavior. Recover the real pointer.
    storedPtrSlot = (void **) ((uintptr_t) ptr - sizeof(void *));
    raw = *storedPtrSlot;
    free(raw);
}

int main(void)
{
    void *simdBuffer = aligned_malloc(DEMO_BUFFER_BYTES, SIMD_ALIGNMENT);
    void *cacheLineBuffer = aligned_malloc(DEMO_BUFFER_BYTES, CACHE_LINE_ALIGNMENT);

    printf("SIMD buffer address: %p (aligned to %d? %s)\n", simdBuffer,
           SIMD_ALIGNMENT,
           (((uintptr_t) simdBuffer % SIMD_ALIGNMENT) == 0) ? "yes" : "no");
    printf("Cache-line buffer address: %p (aligned to %d? %s)\n", cacheLineBuffer,
           CACHE_LINE_ALIGNMENT,
           (((uintptr_t) cacheLineBuffer % CACHE_LINE_ALIGNMENT) == 0) ? "yes" : "no");

    aligned_free(simdBuffer);
    aligned_free(cacheLineBuffer);

    return 0;
}
