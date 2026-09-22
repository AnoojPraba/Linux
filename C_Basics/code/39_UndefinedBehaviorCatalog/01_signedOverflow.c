#include <stdio.h>
#include <limits.h>

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Illustrates signed integer overflow conceptually without actually
 *         triggering it (INT_MAX + 1 on a signed int is undefined behavior
 *         in C - this stays compile-safe by computing in a wider/unsigned
 *         type instead and only printing what WOULD happen).
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    int maxValue = INT_MAX;
    long wideResult = (long)maxValue + 1;

    // illustrative - actual behavior is undefined:
    // "int overflowed = maxValue + 1;" would be UB (not guaranteed to wrap),
    // even though most compilers happen to wrap it in practice today.
    printf("INT_MAX = %d\n", maxValue);
    printf("INT_MAX + 1 computed safely in long = %ld\n", wideResult);
    printf("doing this directly in 'int' arithmetic is undefined behavior\n");

    return 0;
}
