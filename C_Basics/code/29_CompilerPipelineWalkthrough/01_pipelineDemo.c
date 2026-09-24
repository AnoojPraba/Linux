#include <stdio.h>

/*****************************************************************************
 * Name: addOne
 *
 * Description:
 *         Adds one to the given value. Trivial on purpose - this file exists
 *         to be run through each compilation stage, not to demonstrate any
 *         interesting logic.
 *
 * Inputs:
 *         value : the value to increment.
 *
 * Returns:
 *         value + 1.
 *****************************************************************************/
int addOne(int value)
{
    return value + 1;
}

int main(void)
{
    printf("Hello, pipeline! addOne(1) = %d\n", addOne(1));
    return 0;
}
