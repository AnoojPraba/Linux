#include <stdio.h>
#include <stdlib.h>

void allocateAndFill(int **out, int size)
{
    int *buf = malloc(size * sizeof(int));
    int i;

    for (i = 0; i < size; i++)
    {
        buf[i] = i * i;
    }
    *out = buf;
}

int main()
{
    int x    = 5;
    int *p   = &x;
    int **pp = &p;

    printf("x = %d\n", x);
    printf("*p = %d\n", *p);
    printf("**pp = %d\n\n", **pp);

    **pp = 99;
    printf("After **pp = 99, x = %d\n\n", x);

    // A double pointer is genuinely needed here: allocateAndFill must hand back
    // a freshly malloc'd pointer through an out-parameter.
    int *buf = NULL;
    int size = 5;
    int i;

    allocateAndFill(&buf, size);
    for(i = 0; i < size; i++)
    {
        printf("buf[%d] = %d\n", i, buf[i]);
    }
    free(buf);

    return 0;
}
