#include <stdio.h>

// inline is a hint that the compiler may substitute this function's body
// directly at each call site instead of emitting a real call - avoiding
// call/return overhead for small, frequently-used functions. Like
// register (15_StorageClasses/03_registerAuto.c), the compiler is free to
// ignore the hint entirely; modern compilers often inline small static
// functions even without being asked.
static inline int square(int x)
{
    return x * x;
}

// A function this small is exactly the case inline is meant for - the
// call overhead could otherwise dominate the actual one-multiply cost of
// the function body.
int main()
{
    int i;

    for (i = 1; i <= 5; i++)
    {
        printf("square(%d) = %d\n", i, square(i));
    }

    return 0;
}
