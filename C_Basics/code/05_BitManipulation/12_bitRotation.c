#include <stdio.h>

#define BITS_PER_UINT32 32
#define DEMO_VALUE 0x12345678u
#define DEMO_ROTATE_AMOUNT 8

/*****************************************************************************
 * Name: rotateLeft
 *
 * Description:
 *         Rotates n left by k bits. Unlike a plain left shift, which
 *         discards the top k bits off the end, rotation wraps those bits
 *         back around to the bottom: (n << k) shifts the low bits up into
 *         position, and (n >> (32 - k)) brings the bits that would have
 *         been lost back in at the bottom, and the two are ORed together.
 *         This wraparound behavior is used in some hash functions and
 *         cryptographic algorithms, which rely on bits circulating through
 *         every position rather than being discarded.
 *
 * Inputs:
 *         n : the 32-bit value to rotate.
 *         k : number of bit positions to rotate left by.
 *
 * Returns:
 *         n rotated left by k bits.
 *****************************************************************************/
unsigned int rotateLeft(unsigned int n, int k)
{
    return (n << k) | (n >> (BITS_PER_UINT32 - k));
}

/*****************************************************************************
 * Name: rotateRight
 *
 * Description:
 *         Rotates n right by k bits, mirroring rotateLeft: (n >> k) shifts
 *         the high bits down into position, and (n << (32 - k)) brings the
 *         bits that would have been lost back in at the top.
 *
 * Inputs:
 *         n : the 32-bit value to rotate.
 *         k : number of bit positions to rotate right by.
 *
 * Returns:
 *         n rotated right by k bits.
 *****************************************************************************/
unsigned int rotateRight(unsigned int n, int k)
{
    return (n >> k) | (n << (BITS_PER_UINT32 - k));
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates rotateLeft and rotateRight on a sample 32-bit value.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main(void)
{
    unsigned int value = DEMO_VALUE;

    printf("Original:      0x%08x\n", value);
    printf("Rotate left %d: 0x%08x\n", DEMO_ROTATE_AMOUNT,
           rotateLeft(value, DEMO_ROTATE_AMOUNT));
    printf("Rotate right %d:0x%08x\n", DEMO_ROTATE_AMOUNT,
           rotateRight(value, DEMO_ROTATE_AMOUNT));

    return 0;
}
