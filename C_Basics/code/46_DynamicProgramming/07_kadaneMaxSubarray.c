#include <stdio.h>

#define ARR_SIZE 9

/*****************************************************************************
 * Name: maxSubarraySum
 *
 * Description:
 *         Kadane's algorithm: an O(n) single-pass DP for the maximum sum of
 *         any contiguous subarray. bestEndingHere is the max sum of a
 *         subarray that ends exactly at the current index (either just the
 *         current element, or the current element extending the previous
 *         best run); bestOverall tracks the best value seen across all
 *         positions. It is a tiny two-state DP, but it is so commonly asked
 *         on its own in interviews that it is usually taught as its own
 *         named algorithm rather than as "just DP".
 *
 * Inputs:
 *         arr : the input array.
 *         n   : number of elements in arr.
 *
 * Returns:
 *         The maximum contiguous subarray sum.
 *****************************************************************************/
int maxSubarraySum(int arr[], int n)
{
    int bestEndingHere;
    int bestOverall;
    int i;

    bestEndingHere = arr[0];
    bestOverall = arr[0];

    for (i = 1; i < n; i++)
    {
        if (arr[i] > bestEndingHere + arr[i])
        {
            bestEndingHere = arr[i];
        }
        else
        {
            bestEndingHere = bestEndingHere + arr[i];
        }

        if (bestEndingHere > bestOverall)
        {
            bestOverall = bestEndingHere;
        }
    }

    return bestOverall;
}

int main()
{
    int arr[ARR_SIZE] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};

    printf("max subarray sum = %d\n", maxSubarraySum(arr, ARR_SIZE));

    return 0;
}
