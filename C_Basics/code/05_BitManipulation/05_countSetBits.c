#include <stdio.h>

#define BITS_PER_INT 32
#define DEMO_VALUE 29

/*****************************************************************************
 * Name: countSetBitsNaive
 *
 * Description:
 *         Counts set bits by shifting through every bit position and
 *         checking it, regardless of how many bits are actually set.
 *         Runs in O(number of bits) time.
 *
 * Inputs:
 *         n : the value whose set bits are counted.
 *
 * Returns:
 *         The number of set bits in n.
 *****************************************************************************/
int countSetBitsNaive(unsigned int n)
{
    int count = 0;
    int i;

    for (i = 0; i < BITS_PER_INT; i++)
    {
        if ((n >> i) & 1)
        {
            count++;
        }
    }

    return count;
}

/*****************************************************************************
 * Name: countSetBitsKernighan
 *
 * Description:
 *         Brian Kernighan's algorithm: n & (n - 1) clears the lowest set
 *         bit of n on each iteration, so the loop runs exactly once per
 *         set bit. This is O(number of set bits), which beats the naive
 *         O(number of bits) approach whenever n is sparse (few bits set).
 *         The compiler-intrinsic __builtin_popcount(n) is usually the
 *         fastest option in practice, since it can map to a single
 *         hardware POPCNT instruction on supporting CPUs.
 *
 * Inputs:
 *         n : the value whose set bits are counted.
 *
 * Returns:
 *         The number of set bits in n.
 *****************************************************************************/
int countSetBitsKernighan(unsigned int n)
{
    int count = 0;

    while (n)
    {
        n = n & (n - 1);
        count++;
    }

    return count;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates the naive, Kernighan, and compiler-intrinsic
 *         popcount approaches against a single sample value.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main(void)
{
    unsigned int value = DEMO_VALUE;

    printf("Value: %u\n", value);
    printf("Naive popcount:      %d\n", countSetBitsNaive(value));
    printf("Kernighan popcount:  %d\n", countSetBitsKernighan(value));
    printf("Builtin popcount:    %d\n", __builtin_popcount(value));

    return 0;
}
