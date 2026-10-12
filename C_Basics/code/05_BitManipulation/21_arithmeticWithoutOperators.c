// Arithmetic from bit operations only: add, subtract, multiply, divide, and
// compare without the usual operators.

#include <stdio.h>

static int add(int a, int b)
{
    while (b)
    {
        unsigned carry = (unsigned)(a & b) << 1;         /* where both bits are 1 a carry moves left */
        a ^= b;                                          /* sum without carries */
        b = (int)carry;
    }
    return a;
}

static int negate(int a) { return add(~a, 1); }

static int subtract(int a, int b) { return add(a, negate(b)); }

static int multiply(int a, int b)
{
    int sign = (a < 0) ^ (b < 0), result = 0;
    unsigned x = a < 0 ? (unsigned)negate(a) : (unsigned)a, y = b < 0 ? (unsigned)negate(b) : (unsigned)b;
    while (y)                                            /* shift-and-add, like long multiplication in binary */
    {
        if (y & 1) result = add(result, (int)x);
        x <<= 1;
        y >>= 1;
    }
    return sign ? negate(result) : result;
}

static int divide(int a, int b)
{
    int sign = (a < 0) ^ (b < 0);
    unsigned x = a < 0 ? (unsigned)negate(a) : (unsigned)a, y = b < 0 ? (unsigned)negate(b) : (unsigned)b, q = 0;
    for (int i = 31; i >= 0; i--)                        /* binary long division */
        if ((x >> i) >= y)
        {
            x -= y << i;
            q |= 1u << i;
        }
    return sign ? negate((int)q) : (int)q;
}

static int is_equal(int a, int b) { return !(a ^ b); }

int main(void)
{
    printf("add(23, 19) = %d, add(-5, 8) = %d\n", add(23, 19), add(-5, 8));
    printf("subtract(10, 25) = %d\n", subtract(10, 25));
    printf("multiply(7, -6) = %d, multiply(-9, -9) = %d\n", multiply(7, -6), multiply(-9, -9));
    printf("divide(100, 7) = %d, divide(-100, 7) = %d\n", divide(100, 7), divide(-100, 7));
    printf("is_equal(42, 42) = %d, is_equal(42, 43) = %d\n", is_equal(42, 42), is_equal(42, 43));
    return 0;
}
