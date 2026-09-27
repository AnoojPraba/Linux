#include <stdio.h>

#define DEMO_VALUE 0
#define DEMO_BIT_POS 3

/*****************************************************************************
 * Name: setBit
 *
 * Description:
 *         Sets the bit at pos by ORing n with a mask that has only that
 *         bit set, leaving every other bit unchanged.
 *
 * Inputs:
 *         n   : the value to modify.
 *         pos : the bit position to set.
 *
 * Returns:
 *         n with the bit at pos set.
 *****************************************************************************/
unsigned int setBit(unsigned int n, int pos)
{
    return n | (1u << pos);
}

/*****************************************************************************
 * Name: clearBit
 *
 * Description:
 *         Clears the bit at pos by ANDing n with a mask that has every bit
 *         set except pos, leaving every other bit unchanged.
 *
 * Inputs:
 *         n   : the value to modify.
 *         pos : the bit position to clear.
 *
 * Returns:
 *         n with the bit at pos cleared.
 *****************************************************************************/
unsigned int clearBit(unsigned int n, int pos)
{
    return n & ~(1u << pos);
}

/*****************************************************************************
 * Name: toggleBit
 *
 * Description:
 *         Flips the bit at pos by XORing n with a mask that has only that
 *         bit set: a set bit XORed with 1 clears, a clear bit XORed with 1
 *         sets.
 *
 * Inputs:
 *         n   : the value to modify.
 *         pos : the bit position to toggle.
 *
 * Returns:
 *         n with the bit at pos toggled.
 *****************************************************************************/
unsigned int toggleBit(unsigned int n, int pos)
{
    return n ^ (1u << pos);
}

/*****************************************************************************
 * Name: checkBit
 *
 * Description:
 *         Shifts the bit at pos down to position 0, then masks off every
 *         other bit, isolating just that bit's value.
 *
 * Inputs:
 *         n   : the value to inspect.
 *         pos : the bit position to check.
 *
 * Returns:
 *         1 if the bit at pos is set, 0 otherwise.
 *****************************************************************************/
unsigned int checkBit(unsigned int n, int pos)
{
    return (n >> pos) & 1u;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates set/clear/toggle/check on a sample value and bit
 *         position.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main(void)
{
    unsigned int n = DEMO_VALUE;

    n = setBit(n, DEMO_BIT_POS);
    printf("After set bit %d:    0x%x, check = %u\n", DEMO_BIT_POS, n, checkBit(n, DEMO_BIT_POS));

    n = toggleBit(n, DEMO_BIT_POS);
    printf("After toggle bit %d: 0x%x, check = %u\n", DEMO_BIT_POS, n,
           checkBit(n, DEMO_BIT_POS));

    n = setBit(n, DEMO_BIT_POS);
    n = clearBit(n, DEMO_BIT_POS);
    printf("After clear bit %d:  0x%x, check = %u\n", DEMO_BIT_POS, n,
           checkBit(n, DEMO_BIT_POS));

    return 0;
}
