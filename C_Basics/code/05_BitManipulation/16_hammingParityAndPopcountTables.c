// Counting bits several ways: Hamming distance, parity, a lookup table, Brian
// Kernighan and the parallel SWAR popcount.

#include <stdint.h>
#include <stdio.h>

static uint8_t table[256];

static int popcount_table(uint32_t x)
{
    return table[x & 255] + table[x >> 8 & 255] + table[x >> 16 & 255] + table[x >> 24];
}

static int popcount_kernighan(uint32_t x)
{
    int c = 0;
    for (; x; x &= x - 1) c++;                           /* each step clears the lowest set bit */
    return c;
}

static int popcount_swar(uint32_t x)                     /* add neighbouring bit groups in parallel */
{
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    return (int)((x * 0x01010101) >> 24);
}

static int parity(uint32_t x)
{
    x ^= x >> 16; x ^= x >> 8; x ^= x >> 4; x ^= x >> 2; x ^= x >> 1;   /* fold the word onto one bit */
    return x & 1;
}

int main(void)
{
    for (int i = 1; i < 256; i++) table[i] = table[i >> 1] + (i & 1);

    uint32_t v = 0xF0F0A5C3;
    printf("popcount(0x%08X): table %d, kernighan %d, swar %d, builtin %d\n", v, popcount_table(v), popcount_kernighan(v),
           popcount_swar(v), __builtin_popcount(v));
    printf("parity: %d (builtin %d)\n", parity(v), __builtin_parity(v));

    uint32_t a = 0b10110100, b = 0b10011110;
    printf("Hamming distance between 10110100 and 10011110 = %d\n", popcount_swar(a ^ b));
    int agree = 1;
    for (uint32_t x = 0; x < 100000; x++)
        if (popcount_table(x) != popcount_swar(x) || popcount_swar(x) != popcount_kernighan(x)) agree = 0;
    printf("all methods agree for 0..99999: %s\n", agree ? "yes" : "no");
    int total = 0;
    for (int i = 0; i < 256; i++) total += table[i];
    printf("total bits set in 0..255 = %d (8 * 128)\n", total);
    return 0;
}
