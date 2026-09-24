#include <stdio.h>

#define ARRAY_SIZE 8

// 1-indexed Fenwick tree (Binary Indexed Tree); index 0 is unused.
static int fenwick[ARRAY_SIZE + 1];

/*****************************************************************************
 * Name: fenwickUpdate
 *
 * Description:
 *         Adds a delta to a single position and propagates the change to
 *         every ancestor index, found by repeatedly adding the lowbit
 *         (`index & -index`).
 *
 * Inputs:
 *         index : 1-indexed position to update.
 *         delta : amount to add at that position.
 *
 * Returns:
 *         None
 *****************************************************************************/
void fenwickUpdate(int index, int delta)
{
    while (index <= ARRAY_SIZE)
    {
        fenwick[index] = fenwick[index] + delta;
        index = index + (index & -index);
    }
}

/*****************************************************************************
 * Name: fenwickPrefixSum
 *
 * Description:
 *         Computes the sum of elements from index 1 through the given
 *         index, found by repeatedly subtracting the lowbit
 *         (`index & -index`).
 *
 * Inputs:
 *         index : 1-indexed upper bound (inclusive) of the prefix sum.
 *
 * Returns:
 *         Sum of elements in [1, index].
 *****************************************************************************/
int fenwickPrefixSum(int index)
{
    int sum = 0;

    while (index > 0)
    {
        sum = sum + fenwick[index];
        index = index - (index & -index);
    }
    return sum;
}

/*****************************************************************************
 * Name: fenwickRangeSum
 *
 * Description:
 *         Computes the sum of elements over an inclusive 1-indexed range
 *         using two prefix-sum queries.
 *
 * Inputs:
 *         low  : 1-indexed low bound (inclusive) of the range.
 *         high : 1-indexed high bound (inclusive) of the range.
 *
 * Returns:
 *         Sum of elements in [low, high].
 *****************************************************************************/
int fenwickRangeSum(int low, int high)
{
    return fenwickPrefixSum(high) - fenwickPrefixSum(low - 1);
}

int main(void)
{
    int arr[ARRAY_SIZE] = { 1, 3, 5, 7, 9, 11, 13, 15 };
    int i;

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        fenwickUpdate(i + 1, arr[i]);
    }

    printf("sum[2..6] = %d\n", fenwickRangeSum(2, 6));

    fenwickUpdate(4, 100 - arr[3]);
    printf("sum[2..6] after update = %d\n", fenwickRangeSum(2, 6));

    return 0;
}
