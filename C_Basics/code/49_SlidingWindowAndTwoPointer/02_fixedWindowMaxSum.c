#include <stdio.h>

#define ARR_SIZE 8
#define WINDOW_SIZE 3

/*****************************************************************************
 * Name: maxFixedWindowSum
 *
 * Description:
 *         Fixed-size sliding window: maximum sum of any contiguous
 *         subarray of size k. Computes the first window's sum directly,
 *         then slides the window one element at a time by subtracting the
 *         outgoing element and adding the incoming one - O(n) total, versus
 *         O(n*k) if every window's sum were recomputed from scratch.
 *
 * Inputs:
 *         arr : the input array.
 *         n   : number of elements in arr.
 *         k   : the window size.
 *
 * Returns:
 *         The maximum sum of any contiguous subarray of size k.
 *****************************************************************************/
int maxFixedWindowSum(int arr[], int n, int k)
{
    int windowSum;
    int maxSum;
    int i;

    windowSum = 0;

    for (i = 0; i < k; i++)
    {
        windowSum += arr[i];
    }

    maxSum = windowSum;

    for (i = k; i < n; i++)
    {
        windowSum += arr[i] - arr[i - k];

        if (windowSum > maxSum)
        {
            maxSum = windowSum;
        }
    }

    return maxSum;
}

int main()
{
    int arr[ARR_SIZE] = {2, 1, 5, 1, 3, 2, 8, 1};

    printf("max sum of window size %d = %d\n", WINDOW_SIZE,
           maxFixedWindowSum(arr, ARR_SIZE, WINDOW_SIZE));

    return 0;
}
