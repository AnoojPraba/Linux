// A function defined inside another (GCC extension) reading the enclosing
// locals, called directly and through a function pointer (trampoline).

#include <stdio.h>

static int apply_twice(int (*fn)(int), int x)
{
    return fn(fn(x));
}

static int run(int base)
{
    int calls = 0;

    /* GCC extension: a function defined inside another function. */
    int add_base(int v)
    {
        calls++;                    /* may use the enclosing function's locals */
        return v + base;
    }

    int direct = add_base(1);                    /* plain call: no trampoline needed */
    int viaptr = apply_twice(add_base, 10);      /* address taken: GCC builds a trampoline */
    printf("direct=%d via pointer=%d calls=%d\n", direct, viaptr, calls);
    return direct + viaptr;
}

int main(void)
{
    printf("result=%d\n", run(5));
    return 0;
}
