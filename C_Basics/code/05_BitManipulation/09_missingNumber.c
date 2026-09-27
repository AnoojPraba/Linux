#include <stdio.h>

#define ARRAY_N 8

/*****************************************************************************
 * Name: findMissingNumberXor
 *
 * Description:
 *         arr holds n - 1 distinct numbers drawn from the range 1..n, with
 *         exactly one number missing. XORing every array element together
 *         with every number from 1 to n cancels each present value against
 *         its duplicate (x ^ x == 0), leaving only the missing number
 *         (x ^ 0 == x). This avoids the integer overflow that the
 *         alternative sum-formula approach (sum(1..n) minus the actual
 *         array sum) can hit for large n, since XOR never accumulates a
 *         growing magnitude the way addition does.
 *
 * Inputs:
 *         arr : array of n - 1 distinct numbers from 1..n.
 *         n   : the upper bound of the full 1..n range.
 *
 * Returns:
 *         The missing number.
 *****************************************************************************/
int findMissingNumberXor(const int arr[], int n)
{
    int result = 0;
    int i;

    for (i = 0; i < n - 1; i++)
    {
        result = result ^ arr[i];
    }

    for (i = 1; i <= n; i++)
    {
        result = result ^ i;
    }

    return result;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates findMissingNumberXor on a sample array missing the
 *         value 5 out of the range 1..8.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main(void)
{
    int arr[ARRAY_N - 1] = { 1, 2, 3, 4, 6, 7, 8 };

    printf("Missing number: %d\n", findMissingNumberXor(arr, ARRAY_N));

    return 0;
}
