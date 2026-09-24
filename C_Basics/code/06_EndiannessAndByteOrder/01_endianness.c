#include <stdio.h>
#include <arpa/inet.h>

#define TEST_VALUE 0x11223344
#define BYTE_MASK 0xFF
#define BYTE0_SHIFT 24
#define BYTE1_SHIFT 16
#define BYTE2_SHIFT 8
#define NUM_BYTES 4

/*****************************************************************************
 * Name: isLittleEndian
 *
 * Description:
 *         Detects the host's byte order at runtime by storing the integer
 *         value 1 and inspecting whether its low-order byte lands first in
 *         memory (little-endian) or last (big-endian).
 *
 * Returns:
 *         1 if the host is little-endian, 0 if big-endian.
 *****************************************************************************/
int isLittleEndian(void)
{
    int one = 1;
    unsigned char *bytePtr = (unsigned char *) &one;

    return (bytePtr[0] == 1);
}

/*****************************************************************************
 * Name: manualByteSwap32
 *
 * Description:
 *         Byte-swaps a 32-bit value by shifting each byte into its mirrored
 *         position, without relying on any library helper.
 *
 * Inputs:
 *         value : the 32-bit value to byte-swap.
 *
 * Returns:
 *         The byte-swapped value.
 *****************************************************************************/
unsigned int manualByteSwap32(unsigned int value)
{
    unsigned int byte0 = (value >> BYTE0_SHIFT) & BYTE_MASK;
    unsigned int byte1 = (value >> BYTE1_SHIFT) & BYTE_MASK;
    unsigned int byte2 = (value >> BYTE2_SHIFT) & BYTE_MASK;
    unsigned int byte3 = value & BYTE_MASK;

    return (byte3 << BYTE0_SHIFT) | (byte2 << BYTE1_SHIFT) |
           (byte1 << BYTE2_SHIFT) | byte0;
}

/*****************************************************************************
 * Name: printBytes
 *
 * Description:
 *         Prints the raw bytes of a 32-bit value in memory order, to show
 *         the little-endian vs big-endian layout visually.
 *
 * Inputs:
 *         value : the value whose in-memory bytes should be printed.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void printBytes(unsigned int value)
{
    unsigned char *bytePtr = (unsigned char *) &value;
    int i;

    for (i = 0; i < NUM_BYTES; i++)
    {
        printf("%02x ", bytePtr[i]);
    }
    printf("\n");
}

int main(void)
{
    unsigned int manualSwapped = manualByteSwap32(TEST_VALUE);
    unsigned int builtinSwapped = __builtin_bswap32(TEST_VALUE);
    unsigned int hostToNet = htonl(TEST_VALUE);

    printf("Host is %s-endian\n", isLittleEndian() ? "little" : "big");

    printf("Original value:       0x%08x -> bytes: ", TEST_VALUE);
    printBytes(TEST_VALUE);

    printf("Manual byte swap:      0x%08x\n", manualSwapped);
    printf("__builtin_bswap32:     0x%08x\n", builtinSwapped);
    printf("htonl (network order): 0x%08x\n", hostToNet);

    // On a little-endian host, htonl() and the byte-swap results agree
    // because network byte order is defined to be big-endian. On a
    // big-endian host, htonl() would be a no-op instead.
    if ((manualSwapped == builtinSwapped) && (manualSwapped == hostToNet))
    {
        printf("All three swap methods agree.\n");
    }
    else
    {
        printf("Methods disagree - unexpected on this platform.\n");
    }

    return 0;
}
