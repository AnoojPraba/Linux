#include <stdio.h>
#include <limits.h>
#include <ctype.h>

#define ATOI_OK 0
#define ATOI_ERR_INVALID 1
#define ATOI_ERR_OVERFLOW 2
#define ATOI_ERR_UNDERFLOW 3
#define DECIMAL_BASE 10

// Robust string-to-int parser. Overflow/underflow are checked BEFORE the
// multiplication/subtraction that would cause them, since computing an
// out-of-range signed value first and checking afterward is itself
// undefined behavior for signed integers (see 52_UndefinedBehaviorCatalog and NOTES.md here).

/*****************************************************************************
 * Name: safeAtoi
 *
 * Description:
 *         Parses a string into an int, skipping leading whitespace and an
 *         optional sign, and detects overflow/underflow before it happens.
 *
 * Inputs:
 *         str    : NUL-terminated string to parse.
 *         status : out-param set to one of the ATOI_* status codes.
 *
 * Returns:
 *         The parsed value on ATOI_OK, otherwise an unspecified value.
 *****************************************************************************/
int safeAtoi(const char *str, int *status)
{
    int i = 0;
    int sign = 1;
    unsigned int result = 0;
    unsigned int limit;
    int sawDigit = 0;

    while (isspace((unsigned char) str[i]))
    {
        i++;
    }

    if ((str[i] == '+') || (str[i] == '-'))
    {
        if (str[i] == '-')
        {
            sign = -1;
        }
        i++;
    }

    // Working with the unsigned magnitude avoids the INT_MIN trap: the
    // magnitude of INT_MIN (2147483648) does not fit in a signed int, but
    // does fit in an unsigned int, so accumulating here never overflows a
    // signed type.
    limit = (sign == 1) ? (unsigned int) INT_MAX : ((unsigned int) INT_MAX + 1);

    while (isdigit((unsigned char) str[i]))
    {
        unsigned int digit = (unsigned int) (str[i] - '0');

        sawDigit = 1;

        if (result > (limit - digit) / DECIMAL_BASE)
        {
            *status = (sign == 1) ? ATOI_ERR_OVERFLOW : ATOI_ERR_UNDERFLOW;
            return 0;
        }

        result = (result * DECIMAL_BASE) + digit;
        i++;
    }

    if (!sawDigit)
    {
        *status = ATOI_ERR_INVALID;
        return 0;
    }

    *status = ATOI_OK;
    if (sign == 1)
    {
        return (int) result;
    }
    return (int) -result;
}

int main(void)
{
    int status;
    int value;
    const char *tests[] = { "  42", "-2147483648", "2147483648", "abc", "-99" };
    int testCount = sizeof(tests) / sizeof(tests[0]);
    int i;

    for (i = 0; i < testCount; i++)
    {
        value = safeAtoi(tests[i], &status);
        printf("safeAtoi(\"%s\") -> value=%d status=%d\n", tests[i], value, status);
    }

    return 0;
}
