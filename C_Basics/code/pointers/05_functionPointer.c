#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}

int subtract(int a, int b)
{
    return a - b;
}

int applyOp(int a, int b, int (*op)(int, int))
{
    return op(a, b);
}

int main()
{
    int (*fp)(int, int) = add;

    printf("fp(3, 4) via add        = %d\n", fp(3, 4));

    fp = subtract;
    printf("fp(3, 4) via subtract   = %d\n\n", fp(3, 4));

    printf("applyOp(10, 2, add)      = %d\n", applyOp(10, 2, add));
    printf("applyOp(10, 2, subtract) = %d\n", applyOp(10, 2, subtract));

    // An array of function pointers, useful for dispatch tables
    int (*ops[2])(int, int) = {add, subtract};
    printf("\nops[0](5, 5) = %d\n", ops[0](5, 5));
    printf("ops[1](5, 5) = %d\n", ops[1](5, 5));

    return 0;
}
