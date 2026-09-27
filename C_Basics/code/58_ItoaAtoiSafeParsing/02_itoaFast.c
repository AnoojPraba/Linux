#include <stdio.h>
#include <string.h>

#define MIN_BASE 2
#define MAX_BASE 36
#define ITOA_BUF_LEN 34

// Integer-to-string conversion supporting bases 2-36, with correct handling
// of negative numbers (including INT_MIN, whose magnitude does not fit in a
// signed int - see NOTES.md). Digits are written least-significant-first
// into a buffer, then the buffer is reversed once, rather than repeatedly
// prepending (which would be O(n^2)).

/*****************************************************************************
 * Name: itoaFast
 *
 * Description:
 *         Converts an integer to a NUL-terminated string in the given base.
 *
 * Inputs:
 *         value  : the integer to convert.
 *         buffer : destination buffer, must be at least ITOA_BUF_LEN bytes.
 *         base   : the numeric base to convert to, 2-36.
 *
 * Returns:
 *         Pointer to buffer, or NULL if base is out of range.
 *****************************************************************************/
char *itoaFast(int value, char *buffer, int base)
{
    static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    unsigned int magnitude;
    int isNegative = 0;
    int i = 0;
    int start;
    int end;

    if ((base < MIN_BASE) || (base > MAX_BASE))
    {
        return NULL;
    }

    if (value < 0)
    {
        isNegative = 1;
        // Negating INT_MIN directly ("-value") is itself signed overflow UB,
        // since -INT_MIN does not fit in an int. Casting to unsigned first
        // and negating in the unsigned domain (well-defined modular
        // arithmetic) gives the correct magnitude for every value, INT_MIN
        // included.
        magnitude = (unsigned int) 0 - (unsigned int) value;
    }
    else
    {
        magnitude = (unsigned int) value;
    }

    if (magnitude == 0)
    {
        buffer[i++] = '0';
    }

    while (magnitude != 0)
    {
        buffer[i++] = digits[magnitude % (unsigned int) base];
        magnitude /= (unsigned int) base;
    }

    if (isNegative)
    {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Digits were written least-significant-first; reverse in place.
    start = 0;
    end = i - 1;
    while (start < end)
    {
        char tmp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = tmp;
        start++;
        end--;
    }

    return buffer;
}

int main(void)
{
    char buffer[ITOA_BUF_LEN];

    printf("itoaFast(255, base 16) = %s\n", itoaFast(255, buffer, 16));
    printf("itoaFast(-255, base 2) = %s\n", itoaFast(-255, buffer, 2));
    printf("itoaFast(-2147483648, base 10) = %s\n", itoaFast(-2147483648, buffer, 10));
    printf("itoaFast(0, base 10) = %s\n", itoaFast(0, buffer, 10));
    printf("itoaFast(12345, base 36) = %s\n", itoaFast(12345, buffer, 36));

    return 0;
}
