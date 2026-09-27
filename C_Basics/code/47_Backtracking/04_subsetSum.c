#include <stdio.h>

#define SET_SIZE 6
#define TARGET_SUM 9

int gChosen[SET_SIZE];

/*****************************************************************************
 * Name: printChosenSubset
 *
 * Description:
 *         Prints the elements of nums marked as chosen in gChosen.
 *
 * Inputs:
 *         nums : the input set of numbers.
 *         n    : number of elements in nums.
 *****************************************************************************/
void printChosenSubset(int nums[], int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (gChosen[i])
        {
            printf("%d ", nums[i]);
        }
    }

    printf("\n");
}

/*****************************************************************************
 * Name: subsetSum
 *
 * Description:
 *         Backtracking subset-sum search: for each element, try including
 *         it (choose, recurse with a reduced target) then try excluding it
 *         (un-choose, recurse with the same target). Stops as soon as a
 *         qualifying subset is found.
 *
 * Inputs:
 *         nums    : the input set of numbers.
 *         n       : number of elements in nums.
 *         idx     : index of the element currently being decided.
 *         remaining : the sum still needed from nums[idx..n-1].
 *
 * Returns:
 *         1 if some subset of nums[idx..n-1] sums to remaining, 0 otherwise.
 *****************************************************************************/
int subsetSum(int nums[], int n, int idx, int remaining)
{
    if (remaining == 0)
    {
        return 1;
    }

    if ((idx == n) || (remaining < 0))
    {
        return 0;
    }

    gChosen[idx] = 1;

    if (subsetSum(nums, n, idx + 1, remaining - nums[idx]))
    {
        return 1;
    }

    gChosen[idx] = 0;

    return subsetSum(nums, n, idx + 1, remaining);
}

int main()
{
    int nums[SET_SIZE] = {3, 34, 4, 12, 5, 2};

    if (subsetSum(nums, SET_SIZE, 0, TARGET_SUM))
    {
        printf("subset found summing to %d: ", TARGET_SUM);
        printChosenSubset(nums, SET_SIZE);
    }
    else
    {
        printf("no subset sums to %d\n", TARGET_SUM);
    }

    return 0;
}
