// Powers of two and logarithms with bit scans: is power of 2/4, next power of
// two, floor(log2), bit length and integer square root.

#include <stdint.h>
#include <stdio.h>

static int is_pow2(uint32_t x) { return x && !(x & (x - 1)); }
static int is_pow4(uint32_t x) { return is_pow2(x) && (x & 0x55555555); }   /* the single set bit must be at an even position */

static uint32_t next_pow2(uint32_t x)
{
    if (x <= 1) return 1;
    x--;                                                /* smear the highest set bit into every lower position */
    x |= x >> 1; x |= x >> 2; x |= x >> 4; x |= x >> 8; x |= x >> 16;
    return x + 1;
}

static int floor_log2(uint32_t x) { return 31 - __builtin_clz(x); }          /* clz = count leading zeros */
static int bit_length(uint32_t x) { return x ? 32 - __builtin_clz(x) : 0; }

static uint32_t isqrt(uint32_t n)                       /* bitwise integer square root, no floating point */
{
    uint32_t res = 0, bit = 1u << 30;
    while (bit > n) bit >>= 2;
    while (bit)
    {
        if (n >= res + bit) { n -= res + bit; res = (res >> 1) + bit; }
        else res >>= 1;
        bit >>= 2;
    }
    return res;
}

int main(void)
{
    uint32_t tests[] = {1, 2, 3, 16, 64, 100, 1024, 1000000};
    for (int i = 0; i < 8; i++)
    {
        uint32_t x = tests[i];
        printf("%8u: pow2=%d pow4=%d next_pow2=%8u floor_log2=%2d bit_length=%2d isqrt=%4u ctz=%d\n", x, is_pow2(x), is_pow4(x),
               next_pow2(x), floor_log2(x), bit_length(x), isqrt(x), __builtin_ctz(x));
    }
    printf("n %% 8 == n & 7: 77 %% 8 = %d, 77 & 7 = %d\n", 77 % 8, 77 & 7);
    printf("multiply/divide by 2^k with shifts: 13 << 3 = %d, 100 >> 2 = %d\n", 13 << 3, 100 >> 2);
    return 0;
}
