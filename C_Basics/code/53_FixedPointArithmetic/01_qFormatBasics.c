#include <stdio.h>
#include <stdint.h>

#define Q16_16_FRAC_BITS 16

/*****************************************************************************
 * Name: doubleToQ16_16
 *
 * Description:
 *         Converts a double to Q16.16 fixed-point (32-bit signed int, 16
 *         integer bits, 16 fractional bits, scale factor 2^16).
 *
 * Inputs:
 *         value : the double value to convert.
 *
 * Returns:
 *         The Q16.16 fixed-point representation of value.
 *****************************************************************************/
int32_t doubleToQ16_16(double value)
{
    return (int32_t)(value * (1 << Q16_16_FRAC_BITS));
}

/*****************************************************************************
 * Name: q16_16ToDouble
 *
 * Description:
 *         Converts a Q16.16 fixed-point value back to a double. Used only
 *         for testing/printing - real fixed-point code never needs this.
 *
 * Inputs:
 *         value : the Q16.16 fixed-point value to convert.
 *
 * Returns:
 *         The double equivalent of value.
 *****************************************************************************/
double q16_16ToDouble(int32_t value)
{
    return (double)value / (1 << Q16_16_FRAC_BITS);
}

/*****************************************************************************
 * Name: q16_16Add
 *
 * Description:
 *         Adds two Q16.16 values. Addition needs no rescaling since both
 *         operands share the same scale factor.
 *
 * Inputs:
 *         a : the first Q16.16 operand.
 *         b : the second Q16.16 operand.
 *
 * Returns:
 *         a + b in Q16.16.
 *****************************************************************************/
int32_t q16_16Add(int32_t a, int32_t b)
{
    return a + b;
}

/*****************************************************************************
 * Name: q16_16Sub
 *
 * Description:
 *         Subtracts two Q16.16 values. Like addition, no rescaling needed.
 *
 * Inputs:
 *         a : the Q16.16 minuend.
 *         b : the Q16.16 subtrahend.
 *
 * Returns:
 *         a - b in Q16.16.
 *****************************************************************************/
int32_t q16_16Sub(int32_t a, int32_t b)
{
    return a - b;
}

/*****************************************************************************
 * Name: q16_16Mul
 *
 * Description:
 *         Multiplies two Q16.16 values. Multiplying two Q16.16 numbers
 *         directly yields a Q32.32-scaled result, so the raw product is
 *         widened to int64_t before multiplying (to avoid overflow) and
 *         then shifted right by the fractional bit count to rescale back
 *         down to Q16.16.
 *
 * Inputs:
 *         a : the first Q16.16 operand.
 *         b : the second Q16.16 operand.
 *
 * Returns:
 *         a * b in Q16.16.
 *****************************************************************************/
int32_t q16_16Mul(int32_t a, int32_t b)
{
    int64_t product = (int64_t)a * (int64_t)b;
    return (int32_t)(product >> Q16_16_FRAC_BITS);
}

/*****************************************************************************
 * Name: q16_16Div
 *
 * Description:
 *         Divides two Q16.16 values. Dividing raw Q16.16 integers directly
 *         would lose the fractional scale, so the dividend is widened to
 *         int64_t and shifted left by the fractional bit count BEFORE the
 *         division, restoring the correct Q16.16 scale in the quotient.
 *
 * Inputs:
 *         a : the Q16.16 dividend.
 *         b : the Q16.16 divisor.
 *
 * Returns:
 *         a / b in Q16.16.
 *****************************************************************************/
int32_t q16_16Div(int32_t a, int32_t b)
{
    int64_t widenedDividend = (int64_t)a << Q16_16_FRAC_BITS;
    return (int32_t)(widenedDividend / b);
}

int main(void)
{
    int32_t a = doubleToQ16_16(3.5);
    int32_t b = doubleToQ16_16(2.25);
    int32_t sum = q16_16Add(a, b);
    int32_t diff = q16_16Sub(a, b);
    int32_t product = q16_16Mul(a, b);
    int32_t quotient = q16_16Div(a, b);

    printf("a = %f, b = %f\n", q16_16ToDouble(a), q16_16ToDouble(b));
    printf("fixed add:      %f (float: %f)\n", q16_16ToDouble(sum), 3.5 + 2.25);
    printf("fixed sub:      %f (float: %f)\n", q16_16ToDouble(diff), 3.5 - 2.25);
    printf("fixed mul:      %f (float: %f)\n", q16_16ToDouble(product), 3.5 * 2.25);
    printf("fixed div:      %f (float: %f)\n", q16_16ToDouble(quotient), 3.5 / 2.25);

    return 0;
}
