#include <stdio.h>
#include "libgreet.h"

/*****************************************************************************
 * Name: addNumbers
 *
 * Description:
 *         Adds two integers - trivial exported symbol used to demonstrate
 *         linking against this library both statically and dynamically.
 *
 * Inputs:
 *         a : first operand.
 *         b : second operand.
 *
 * Returns:
 *         Sum of a and b.
 *****************************************************************************/
int addNumbers(int a, int b)
{
    return a + b;
}

/*****************************************************************************
 * Name: greet
 *
 * Description:
 *         Prints a greeting for the given name.
 *
 * Inputs:
 *         name : the name to greet.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void greet(const char *name)
{
    printf("Hello, %s! (from libgreet)\n", name);
}
