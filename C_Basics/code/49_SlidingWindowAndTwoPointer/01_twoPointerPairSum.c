#include <stdio.h>

#define ARR_SIZE 7
#define TARGET_SUM 12

/*****************************************************************************
 * Name: hasPairWithSum
 *
 * Description:
 *         Opposite-direction two-pointer search for a pair summing to
 *         target in a SORTED array: start with left at 0 and right at the
 *         last index, and shrink whichever side won't help - if the current
 *         sum is too small, only moving left forward can increase it; if
 *         too large, only moving right backward can decrease it. O(n) and
 *         O(1) extra space, versus O(n^2) checking every pair with brute
 *         force, or O(n) time / O(n) space using a hash set (which also
 *         works on unsorted input, unlike this two-pointer approach which
 *         requires the sorted precondition).
 *
 * Inputs:
 *         arr    : a SORTED array of integers.
 *         n      : number of elements in arr.
 *         target : the target sum to find.
 *
 * Returns:
 *         1 if some pair sums to target, 0 otherwise.
 *****************************************************************************/
int hasPairWithSum(int arr[], int n, int target)
{
    int left;
    int right;

    left = 0;
    right = n - 1;

    while (left < right)
    {
        int sum = arr[left] + arr[right];

        if (sum == target)
        {
            printf("pair found: %d + %d = %d\n", arr[left], arr[right], target);
            return 1;
        }
        else if (sum < target)
        {
            left++;
        }
        else
        {
            right--;
        }
    }

    return 0;
}

int main()
{
    int arr[ARR_SIZE] = {1, 2, 4, 5, 7, 9, 11};

    if (!hasPairWithSum(arr, ARR_SIZE, TARGET_SUM))
    {
        printf("no pair sums to %d\n", TARGET_SUM);
    }

    return 0;
}
