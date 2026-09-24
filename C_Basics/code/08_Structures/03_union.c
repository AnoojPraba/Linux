#include <stdio.h>

union Value
{
    int i;
    float f;
    char bytes[4];
};

int main()
{
    union Value v;

    // All members of a union share the same memory - writing one member
    // overwrites the others, unlike a struct where each member has its own space.
    v.i = 65;
    printf("v.i = %d\n", v.i);
    printf("v.bytes[0] = %d (same memory as v.i's low byte)\n\n", v.bytes[0]);

    v.f = 3.14f;
    printf("v.f = %f\n", v.f);
    printf("v.i (now garbage, reinterpreted as int) = %d\n\n", v.i);

    printf("sizeof(union Value) = %lu (size of its largest member)\n",
           (unsigned long)sizeof(union Value));

    return 0;
}
