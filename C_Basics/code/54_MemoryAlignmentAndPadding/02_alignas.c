#include <stdio.h>
#include <stdalign.h>

#define CACHE_LINE_SIZE 64

// alignas() forces a stronger alignment than the type would naturally get -
// commonly used to align a hot variable to its own cache line (see
// ../../OS/code/38_FalseSharing for why that matters).
typedef struct
{
    alignas(CACHE_LINE_SIZE) long counter;
} CacheLineAligned;

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates alignas() forcing a struct onto a cache-line boundary
 *         and confirms it with alignof().
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    CacheLineAligned aligned;

    printf("alignof(CacheLineAligned) = %zu (forced to cache line size)\n",
           alignof(CacheLineAligned));
    printf("address of aligned.counter = %p\n", (void *)&aligned.counter);
    printf("address %% %d = %lu (0 means cache-line aligned)\n", CACHE_LINE_SIZE,
           (unsigned long)((size_t)&aligned.counter % CACHE_LINE_SIZE));

    return 0;
}
