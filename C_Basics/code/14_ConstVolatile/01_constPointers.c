#include <stdio.h>

int main()
{
    int a = 10;
    int b = 20;

    // pointer to const int: cannot modify *pc, but pc itself can be reassigned
    const int *pc = &a;

    pc = &b;
    printf("*pc = %d\n", *pc);

    // const pointer to int: cp must always point at c, but *cp can be modified
    int c = 30;
    int *const cp = &c;

    *cp = 99;
    printf("*cp = %d\n", *cp);

    // const pointer to const int: neither the pointer nor the pointee can change
    const int *const cpc = &a;

    printf("*cpc = %d\n", *cpc);

    return 0;
}
