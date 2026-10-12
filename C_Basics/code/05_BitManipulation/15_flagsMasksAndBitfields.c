// Practical bit flags: permission masks, setting/clearing/toggling, packing
// several fields in one integer and endianness-independent extraction.

#include <stdint.h>
#include <stdio.h>

enum { READ = 1 << 0, WRITE = 1 << 1, EXEC = 1 << 2 };

static void show(const char *who, unsigned perm)
{
    printf("%-6s %c%c%c (%u)\n", who, perm & READ ? 'r' : '-', perm & WRITE ? 'w' : '-', perm & EXEC ? 'x' : '-', perm);
}

/* pack a date into 16 bits: year since 1980 (7 bits) | month (4 bits) | day (5 bits), like FAT directory entries */
static uint16_t pack_date(int year, int month, int day)
{
    return (uint16_t)((year - 1980) << 9 | month << 5 | day);
}

int main(void)
{
    unsigned perm = READ | WRITE;
    show("owner", perm);
    perm |= EXEC;                                       /* set */
    show("+x", perm);
    perm &= ~WRITE;                                     /* clear */
    show("-w", perm);
    perm ^= READ;                                       /* toggle */
    show("^r", perm);
    printf("has EXEC: %d, has WRITE: %d, all of READ|EXEC: %d\n", !!(perm & EXEC), !!(perm & WRITE), (perm & (READ | EXEC)) == (READ | EXEC));

    uint16_t d = pack_date(2026, 10, 12);
    printf("packed date 0x%04X -> year %d month %d day %d\n", d, (d >> 9) + 1980, d >> 5 & 15, d & 31);

    uint32_t word = 0x12345678;
    printf("bytes by shifting (any endianness): %02X %02X %02X %02X\n", word >> 24, word >> 16 & 255, word >> 8 & 255, word & 255);
    uint8_t *bytes = (uint8_t *)&word;
    printf("bytes by memory order here: %02X %02X %02X %02X (%s endian)\n", bytes[0], bytes[1], bytes[2], bytes[3], bytes[0] == 0x78 ? "little" : "big");

    unsigned rgb = 0xFF8040;                            /* extract and rebuild colour channels */
    unsigned r = rgb >> 16 & 255, g = rgb >> 8 & 255, b = rgb & 255;
    printf("rgb 0x%06X -> r=%u g=%u b=%u, brighter: 0x%06X\n", rgb, r, g, b, (r > 200 ? 255 : r + 55) << 16 | (g + 55) << 8 | (b + 55));
    return 0;
}
