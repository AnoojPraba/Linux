// Gray code: generate it, convert binary to Gray and back, and see why
// neighbouring codes differ in one bit.

#include <stdio.h>

static unsigned to_gray(unsigned n) { return n ^ (n >> 1); }

static unsigned from_gray(unsigned g)
{
    unsigned n = 0;
    for (; g; g >>= 1) n ^= g;                          /* prefix xor of the bits */
    return n;
}

static void print_bits(unsigned x, int width)
{
    for (int i = width - 1; i >= 0; i--) putchar(x >> i & 1 ? '1' : '0');
}

int main(void)
{
    printf("n  binary  gray   differs from previous in\n");
    for (unsigned n = 0; n < 8; n++)
    {
        unsigned g = to_gray(n);
        printf("%u  ", n);
        print_bits(n, 3);
        printf("     ");
        print_bits(g, 3);
        printf("    ");
        if (n) printf("%d bit", __builtin_popcount(g ^ to_gray(n - 1)));
        printf("\n");
    }
    int ok = 1;
    for (unsigned n = 0; n < 1000; n++)
        if (from_gray(to_gray(n)) != n) ok = 0;
    printf("round trip gray -> binary for 0..999: %s\n", ok ? "ok" : "broken");

    printf("reflected construction for 3 bits:");        /* G(n) = 0*G(n-1) followed by 1*reverse(G(n-1)) */
    unsigned list[8] = {0, 1}, len = 2;
    for (int bit = 1; bit < 3; bit++)
    {
        for (unsigned i = 0; i < len; i++) list[len + i] = list[len - 1 - i] | (1u << bit);
        len *= 2;
    }
    for (unsigned i = 0; i < 8; i++) printf(" %u", list[i]);
    printf("\n");
    return 0;
}
