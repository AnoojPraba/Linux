#include <stdio.h>

// _Generic (C11) picks a branch at compile time based on the type of its
// controlling expression - this is compile-time dispatch, not a runtime
// switch, so there is no overhead and no need for function overloading
// (which C, unlike C++, doesn't have).
#define add(a, b) _Generic((a), \
    int: addInt, \
    float: addFloat, \
    double: addDouble)(a, b)

int addInt(int a, int b)
{
    return a + b;
}

float addFloat(float a, float b)
{
    return a + b;
}

double addDouble(double a, double b)
{
    return a + b;
}

// A type-generic "describe" macro is the other common use of _Generic:
// picking a format description instead of a function.
#define typeName(x) _Generic((x), \
    int: "int", \
    float: "float", \
    double: "double", \
    char *: "char *", \
    default: "unknown")

int main()
{
    int i = 2;
    float f = 2.5f;
    double d = 3.5;

    printf("add(2, 3) = %d\n", add(i, 3));
    printf("add(2.5f, 1.5f) = %f\n", add(f, 1.5f));
    printf("add(3.5, 1.5) = %f\n\n", add(d, 1.5));

    printf("typeName(i) = %s\n", typeName(i));
    printf("typeName(f) = %s\n", typeName(f));
    printf("typeName(d) = %s\n", typeName(d));

    return 0;
}
