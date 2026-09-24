#include <stdio.h>

#define OP_ADD 0
#define OP_SUB 1
#define OP_MUL 2
#define NUM_OPS 3

int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

int mul(int a, int b)
{
    return a * b;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Builds a function pointer dispatch table indexed by opcode and
 *         invokes each entry, avoiding a long if/else or switch chain.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    int (*dispatch[NUM_OPS])(int, int) = { add, sub, mul };
    const char *names[NUM_OPS] = { "add", "sub", "mul" };
    int i;

    for (i = 0; i < NUM_OPS; i++)
    {
        printf("%s(6, 3) = %d\n", names[i], dispatch[i](6, 3));
    }

    return 0;
}
