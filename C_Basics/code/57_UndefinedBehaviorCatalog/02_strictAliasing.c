#include <stdio.h>
#include <string.h>

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Illustrates the strict-aliasing rule: reading an object through a
 *         pointer of an incompatible type (e.g. reinterpreting a float* as
 *         an int*) is undefined behavior. Stays compile-safe by using
 *         memcpy(), the standard sanctioned way to reinterpret bytes.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    float floatValue = 3.14f;
    int bitPattern;

    // illustrative - actual behavior is undefined:
    // "int bits = *(int *)&floatValue;" violates strict aliasing because an
    // int lvalue is used to read an object actually stored as a float.
    memcpy(&bitPattern, &floatValue, sizeof(bitPattern));

    printf("float %f has bit pattern 0x%08x (read safely via memcpy)\n",
           floatValue, (unsigned int)bitPattern);

    return 0;
}
