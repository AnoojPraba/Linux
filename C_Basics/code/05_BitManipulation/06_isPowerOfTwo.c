#include <stdio.h>

#define DEMO_POWER_OF_TWO 64
#define DEMO_NON_POWER_OF_TWO 100

/*****************************************************************************
 * Name: isPowerOfTwo
 *
 * Description:
 *         A power of two has exactly one bit set (e.g. 8 = 0b1000).
 *         Subtracting 1 flips that single set bit to 0 and sets every
 *         lower bit to 1 (e.g. 7 = 0b0111). ANDing the two together then
 *         clears the one bit n had set, leaving zero. Any n that is not a
 *         power of two has more than one bit set, so the AND leaves at
 *         least one bit standing. n must also be positive, since 0 would
 *         otherwise incorrectly pass the AND check.
 *
 * Inputs:
 *         n : the value to test.
 *
 * Returns:
 *         1 if n is a power of two, 0 otherwise.
 *****************************************************************************/
int isPowerOfTwo(int n)
{
    return (n > 0) && ((n & (n - 1)) == 0);
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates isPowerOfTwo against a power-of-two value and a
 *         non-power-of-two value.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main(void)
{
    printf("%d is power of two: %d\n", DEMO_POWER_OF_TWO, isPowerOfTwo(DEMO_POWER_OF_TWO));
    printf("%d is power of two: %d\n", DEMO_NON_POWER_OF_TWO,
           isPowerOfTwo(DEMO_NON_POWER_OF_TWO));
    printf("0 is power of two: %d\n", isPowerOfTwo(0));

    return 0;
}
