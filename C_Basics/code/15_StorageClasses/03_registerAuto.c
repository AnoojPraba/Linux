#include <stdio.h>

int main()
{
    // auto is the default storage class for a block-scope variable; writing
    // it explicitly is legal but essentially never done in modern C.
    auto int a = 5;

    // register is a hint that a variable should live in a CPU register for
    // fast access; the compiler is free to ignore it, and you cannot take
    // its address (&r would be an error).
    register int r = 10;
    int sum = a + r;

    printf("a = %d, r = %d, sum = %d\n", a, r, sum);

    return 0;
}
