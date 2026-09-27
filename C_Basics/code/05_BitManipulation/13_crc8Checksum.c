#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CRC8_POLY 0x07
#define CRC8_INIT 0x00
#define BITS_PER_BYTE 8
#define MSB_MASK 0x80

/*****************************************************************************
 * Name: crc8Compute
 *
 * Description:
 *         Computes a CRC-8 checksum over a byte buffer using the classic
 *         bitwise algorithm: each message byte is XORed into the top of the
 *         CRC register, then the register is shifted left 8 times, XORing
 *         in the polynomial whenever the bit shifted out was 1. Uses
 *         polynomial 0x07 (x^8 + x^2 + x + 1), a common CRC-8 choice.
 *
 *         A table-driven version (a precomputed 256-entry lookup table
 *         indexed by the current byte) trades a small ROM table for far
 *         fewer runtime operations - the common approach in real embedded
 *         communication stacks (UART/CAN framing, etc.) where this bitwise
 *         loop would be too slow. Not implemented here for simplicity.
 *
 * Inputs:
 *         data   : pointer to the message bytes.
 *         length : number of bytes in the message.
 *
 * Returns:
 *         The 8-bit CRC checksum of the message.
 *****************************************************************************/
uint8_t crc8Compute(const uint8_t *data, size_t length)
{
    uint8_t crc = CRC8_INIT;
    size_t i;
    int bit;

    for (i = 0; i < length; i++)
    {
        crc = crc ^ data[i];
        for (bit = 0; bit < BITS_PER_BYTE; bit++)
        {
            if ((crc & MSB_MASK) != 0)
            {
                crc = (uint8_t)((crc << 1) ^ CRC8_POLY);
            }
            else
            {
                crc = (uint8_t)(crc << 1);
            }
        }
    }
    return crc;
}

int main(void)
{
    uint8_t message[] = "Hello, embedded!";
    uint8_t corrupted[sizeof(message)];
    size_t length = strlen((const char *)message);
    uint8_t originalCrc;
    uint8_t corruptedCrc;

    memcpy(corrupted, message, sizeof(message));

    originalCrc = crc8Compute(message, length);
    printf("message:        \"%s\"\n", message);
    printf("CRC-8:          0x%02X\n", originalCrc);

    // Flip a single bit in a copy of the message to simulate transmission
    // corruption, then recompute the CRC to show it now differs.
    corrupted[0] = corrupted[0] ^ 0x01;
    corruptedCrc = crc8Compute(corrupted, length);
    printf("corrupted:      \"%s\" (first byte, bit 0 flipped)\n", corrupted);
    printf("CRC-8:          0x%02X\n", corruptedCrc);

    if (originalCrc != corruptedCrc)
    {
        printf("corruption detected: CRCs differ, as expected.\n");
    }
    else
    {
        printf("corruption NOT detected (unexpected).\n");
    }

    return 0;
}
