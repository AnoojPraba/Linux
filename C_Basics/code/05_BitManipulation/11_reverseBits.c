#include <stdio.h>

#define BITS_PER_UINT32 32
#define DEMO_VALUE 43261596u

/*****************************************************************************
 * Name: reverseBits
 *
 * Description:
 *         Reverses the bit order of a 32-bit unsigned integer by walking
 *         through all 32 bits of n, and for each one shifting the running
 *         result left to make room, then ORing in n's next lowest bit.
 *         The first bit processed (n's least significant bit) ends up as
 *         the result's most significant bit, and so on, mirroring the
 *         whole word.
 *
 * Inputs:
 *         n : the 32-bit value to reverse.
 *
 * Returns:
 *         n with its bit order reversed.
 *****************************************************************************/
unsigned int reverseBits(unsigned int n)
{
    unsigned int result = 0;
    int i;

    for (i = 0; i < BITS_PER_UINT32; i++)
    {
        result = (result << 1) | (n & 1);
        n = n >> 1;
    }

    return result;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates reverseBits on a sample 32-bit value.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main(void)
{
    unsigned int value = DEMO_VALUE;

    printf("Original: 0x%08x\n", value);
    printf("Reversed: 0x%08x\n", reverseBits(value));

    return 0;
}
