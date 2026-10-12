// A catalog of one-line bit tricks: lowest set bit, clear lowest, isolate,
// branch-free min/max/abs, sign, swap, average without overflow and bit-field
// extraction.

#include <stdint.h>
#include <stdio.h>

static void bin(const char *label, uint32_t x)
{
    printf("%-26s %3u = ", label, x);
    for (int i = 7; i >= 0; i--) putchar(x >> i & 1 ? '1' : '0');
    putchar('\n');
}

int main(void)
{
    uint32_t x = 0b01101000;
    bin("x", x);
    bin("x & -x (lowest set bit)", x & -x);
    bin("x & (x - 1) (clear lowest)", x & (x - 1));
    bin("x | (x - 1) (set trailing)", x | (x - 1));
    bin("~x & (x + 1) (lowest zero)", ~x & (x + 1));
    bin("x ^ (x >> 1) (Gray code)", x ^ (x >> 1));

    int a = 7, b = -12;
    int mask = (a - b) >> 31;                           /* all ones if a < b (arithmetic shift) */
    printf("branch-free min(7, -12) = %d, max = %d\n", b + ((a - b) & mask), a - ((a - b) & mask));
    int s = b >> 31;
    printf("abs(-12) = %d (xor/subtract the sign mask)\n", (b ^ s) - s);
    printf("sign(-12) = %d, sign(7) = %d, sign(0) = %d\n", (b > 0) - (b < 0), (a > 0) - (a < 0), (0 > 0) - (0 < 0));

    a ^= b; b ^= a; a ^= b;                             /* xor swap (breaks if both operands are the same object) */
    printf("after xor swap a = %d, b = %d\n", a, b);

    unsigned p = 4000000000u, q = 3000000000u;
    printf("average without overflow: %u (naive (p+q)/2 would wrap)\n", (p & q) + ((p ^ q) >> 1));

    uint32_t reg = 0xABCD1234;
    printf("bits 8..15 of 0xABCD1234 = 0x%02X, replace them with 0xFF: 0x%08X\n",
           (reg >> 8) & 0xFF, (reg & ~(0xFFu << 8)) | (0xFFu << 8));
    printf("is power of two: 64 -> %d, 96 -> %d, 0 -> %d (the zero case needs the extra test)\n",
           (64 & 63) == 0 && 64, (96 & 95) == 0 && 96, (0 & -1) == 0 && 0);
    printf("round 37 up to a multiple of 8: %d, down: %d\n", (37 + 7) & ~7, 37 & ~7);
    return 0;
}
