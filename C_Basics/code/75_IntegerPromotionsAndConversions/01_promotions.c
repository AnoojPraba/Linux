#include <limits.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    // 1. Signed/unsigned comparison: -1 converts to UINT_MAX.
    int s = -1;
    unsigned int u = 1;
    printf("(-1 < 1u)       = %d   <- surprising: -1 became %u\n", s < u, (unsigned)s);

    // 2. Classic loop bug: size_t is unsigned, so i >= 0 is always true.
    //    Safe reverse loop idiom shown instead of the buggy one.
    size_t n = 3;
    printf("reverse loop:");
    for (size_t i = n; i-- > 0;)
        printf(" %zu", i);
    printf("\n");

    // 3. Integer promotion: char/short operands become int before arithmetic.
    unsigned char a = 200, b = 100;
    printf("a + b           = %d  (computed in int, no wrap at 255)\n", a + b);
    unsigned char c = a + b;
    printf("stored in uchar = %d  (implementation-defined narrowing: wraps)\n", c);

    // 4. ~ on a small unsigned type promotes first.
    unsigned char m = 0x0F;
    printf("~m              = 0x%X  (int, not 0xF0)\n", ~m);
    printf("(uint8_t)~m     = 0x%X\n", (uint8_t)~m);

    // 5. Shifting a promoted value: m is int here, so << stays defined
    //    until it hits the sign bit (UB for signed overflow of result).
    uint8_t hi = 0x80;
    printf("hi << 1         = 0x%X  (int, bit 8 survives)\n", hi << 1);

    // 6. Usual arithmetic conversions: long vs unsigned int on LP64 -> long.
    long L = -1;
    unsigned int U = 1;
    printf("(-1L < 1u)      = %d  (long is wider, so no wrap on LP64)\n", L < U);

    // 7. Signed overflow is UB; unsigned wraps by definition.
    unsigned int wrap = UINT_MAX;
    wrap += 1;
    printf("UINT_MAX + 1    = %u  (defined)\n", wrap);

    // 8. Float -> int truncates toward zero; out-of-range is UB.
    printf("(int)-2.9       = %d\n", (int)-2.9);

    // 9. abs(INT_MIN) is UB. Guard before negating.
    printf("-INT_MIN safe?  : check x == INT_MIN before negating\n");

    // 10. char signedness is implementation-defined (signed on x86, unsigned
    //     on ARM Linux). Cast to unsigned char before indexing tables/ctype.
    char ch = (char)0xE9;
    printf("char 0xE9 as int = %d (platform dependent)\n", ch);
    printf("via unsigned char = %d (portable)\n", (unsigned char)ch);
    return 0;
}
