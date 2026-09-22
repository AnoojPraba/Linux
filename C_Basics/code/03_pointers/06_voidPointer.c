#include <stdio.h>

// A void pointer holds an address but no type information, so it must be
// cast to a concrete pointer type before it can be dereferenced.
void printAsInt(void *p)
{
    int *ip = (int *)p;

    printf("as int: %d\n", *ip);
}

void printAsFloat(void *p)
{
    float *fp = (float *)p;

    printf("as float: %f\n", *fp);
}

int main()
{
    int i = 42;
    float f = 3.14f;
    void *generic;

    generic = &i;
    printAsInt(generic);

    generic = &f;
    printAsFloat(generic);

    // sizeof(void) is undefined by the standard, and void* cannot be
    // dereferenced directly - printf("%d", *generic) would not compile.
    printf("sizeof(void *) = %lu\n", (unsigned long)sizeof(void *));

    return 0;
}
