#include <stdio.h>
#include <stdarg.h>

// The count of arguments must be passed explicitly (or via a sentinel value) -
// there is no way for a variadic function to know how many arguments it
// received from va_list alone.
int sumInts(int count, ...)
{
    va_list args;
    int total = 0;
    int i;

    va_start(args, count);
    for (i = 0; i < count; i++)
    {
        total += va_arg(args, int);
    }
    va_end(args);

    return total;
}

int main()
{
    printf("sumInts(3, 1, 2, 3) = %d\n", sumInts(3, 1, 2, 3));
    printf("sumInts(5, 10, 20, 30, 40, 50) = %d\n", sumInts(5, 10, 20, 30, 40, 50));

    return 0;
}
