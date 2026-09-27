#include <stdio.h>

#define NUM_STAIRS 5

/*****************************************************************************
 * Name: climbStairs
 *
 * Description:
 *         Counts the distinct ways to climb n stairs taking 1 or 2 steps at
 *         a time. The number of ways to reach step i is the sum of the ways
 *         to reach step i-1 (then take one step) and step i-2 (then take
 *         two steps) - this recurrence is exactly Fibonacci in disguise, so
 *         it's computed the same way as bottom-up DP/tabulation (see
 *         46_DynamicProgramming/01_fibMemoVsTabulation.c), just with two
 *         running values instead of an array since only the last two
 *         results are ever needed.
 *
 * Inputs:
 *         n : number of stairs.
 *
 * Returns:
 *         Number of distinct ways to climb n stairs.
 *****************************************************************************/
int climbStairs(int n)
{
    int prev;
    int curr;
    int next;
    int i;

    if (n <= 1)
    {
        return 1;
    }

    prev = 1;
    curr = 1;
    for (i = 2; i <= n; i++)
    {
        next = prev + curr;
        prev = curr;
        curr = next;
    }

    return curr;
}

int main(void)
{
    printf("ways to climb %d stairs = %d\n", NUM_STAIRS, climbStairs(NUM_STAIRS));

    return 0;
}
