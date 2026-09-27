#include <stdio.h>

#define DEMO_A 5
#define DEMO_B 9

/*****************************************************************************
 * Name: xorSwap
 *
 * Description:
 *         Swaps *a and *b without a temporary variable, using XOR's
 *         self-inverse property (x ^ x == 0 and x ^ 0 == x):
 *             *a = *a ^ *b;   // *a now holds (a ^ b)
 *             *b = *a ^ *b;   // (a ^ b) ^ b == a, so *b becomes original a
 *             *a = *a ^ *b;   // (a ^ b) ^ a == b, so *a becomes original b
 *         This is a fun interview trick, but modern compilers already turn
 *         a plain temp-variable swap into equally fast (or faster) code,
 *         so there is no real-world performance reason to use it.
 *
 * Special Considerations:
 *         Fails if a and b point to the same memory location: the first
 *         XOR computes x ^ x, which is 0, zeroing out the shared value
 *         before the swap can complete, so the variable ends up as 0
 *         instead of unchanged.
 *
 * Inputs:
 *         a : pointer to the first value.
 *         b : pointer to the second value.
 *****************************************************************************/
void xorSwap(int *a, int *b)
{
    if (a == b)
    {
        return;
    }

    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates xorSwap on two distinct integers.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main(void)
{
    int a = DEMO_A;
    int b = DEMO_B;

    printf("Before swap: a = %d, b = %d\n", a, b);
    xorSwap(&a, &b);
    printf("After swap:  a = %d, b = %d\n", a, b);

    return 0;
}
