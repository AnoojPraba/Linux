#include <stdio.h>

#define ARRAY_SIZE 4096
#define STRIDE_FAR 4096
#define ITERATIONS 100000

/*****************************************************************************
 * Name: sumSequential
 *
 * Description:
 *         Sums an array walking it sequentially - cache-friendly access
 *         pattern (each cache line fetched is fully used before moving on).
 *
 * Inputs:
 *         array : the array to sum.
 *         size  : number of elements in array.
 *
 * Returns:
 *         Sum of all elements.
 *****************************************************************************/
long sumSequential(int *array, int size)
{
    long total = 0;
    int i;

    for (i = 0; i < size; i++)
    {
        total += array[i];
    }
    return total;
}

/*****************************************************************************
 * Name: sumStrided
 *
 * Description:
 *         Sums the same array but walking with a large stride that jumps
 *         far outside the current cache line on every access - deliberately
 *         cache-unfriendly, to give `perf stat` a visible difference in
 *         cache-misses / instructions-per-cycle versus sumSequential().
 *
 * Inputs:
 *         array : the array to sum.
 *         size  : number of elements in array.
 *
 * Returns:
 *         Sum of all elements.
 *****************************************************************************/
long sumStrided(int *array, int size)
{
    long total = 0;
    int i;
    int index;

    for (i = 0; i < size; i++)
    {
        index = (i * STRIDE_FAR) % size;
        total += array[index];
    }
    return total;
}

int main()
{
    int array[ARRAY_SIZE];
    int i;
    long sequentialTotal = 0;
    long stridedTotal = 0;

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = i;
    }

    // Run each loop many times so `perf stat`/`time` have enough duration
    // to produce stable counters - see NOTES.md for the actual commands
    // and what to expect to differ between the two.
    for (i = 0; i < ITERATIONS; i++)
    {
        sequentialTotal += sumSequential(array, ARRAY_SIZE);
    }
    for (i = 0; i < ITERATIONS; i++)
    {
        stridedTotal += sumStrided(array, ARRAY_SIZE);
    }

    printf("sequentialTotal = %ld\n", sequentialTotal);
    printf("stridedTotal    = %ld\n", stridedTotal);

    return 0;
}
