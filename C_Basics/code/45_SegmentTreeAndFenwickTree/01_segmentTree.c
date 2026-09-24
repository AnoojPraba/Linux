#include <stdio.h>

#define ARRAY_SIZE 8
#define TREE_SIZE (4 * ARRAY_SIZE)

static int tree[TREE_SIZE];

/*****************************************************************************
 * Name: buildSegmentTree
 *
 * Description:
 *         Recursively builds a range-sum segment tree over an array
 *         segment, storing each node's sum at treeIndex.
 *
 * Inputs:
 *         arr        : source array.
 *         treeIndex  : index of this node within the tree array.
 *         low        : low bound (inclusive) of this node's range.
 *         high       : high bound (inclusive) of this node's range.
 *
 * Returns:
 *         None
 *****************************************************************************/
void buildSegmentTree(const int *arr, int treeIndex, int low, int high)
{
    int mid;

    if (low == high)
    {
        tree[treeIndex] = arr[low];
        return;
    }

    mid = low + (high - low) / 2;
    buildSegmentTree(arr, 2 * treeIndex + 1, low, mid);
    buildSegmentTree(arr, 2 * treeIndex + 2, mid + 1, high);
    tree[treeIndex] = tree[2 * treeIndex + 1] + tree[2 * treeIndex + 2];
}

/*****************************************************************************
 * Name: updateSegmentTree
 *
 * Description:
 *         Updates the value at a single array position and propagates the
 *         change up through the affected tree nodes.
 *
 * Inputs:
 *         treeIndex : index of this node within the tree array.
 *         low       : low bound (inclusive) of this node's range.
 *         high      : high bound (inclusive) of this node's range.
 *         position  : array index being updated.
 *         value     : new value at position.
 *
 * Returns:
 *         None
 *****************************************************************************/
void updateSegmentTree(int treeIndex, int low, int high, int position, int value)
{
    int mid;

    if (low == high)
    {
        tree[treeIndex] = value;
        return;
    }

    mid = low + (high - low) / 2;
    if (position <= mid)
    {
        updateSegmentTree(2 * treeIndex + 1, low, mid, position, value);
    }
    else
    {
        updateSegmentTree(2 * treeIndex + 2, mid + 1, high, position, value);
    }
    tree[treeIndex] = tree[2 * treeIndex + 1] + tree[2 * treeIndex + 2];
}

/*****************************************************************************
 * Name: querySegmentTree
 *
 * Description:
 *         Computes the sum over a query range using the segment tree,
 *         returning early on ranges fully outside or fully inside the
 *         current node's range.
 *
 * Inputs:
 *         treeIndex  : index of this node within the tree array.
 *         low        : low bound (inclusive) of this node's range.
 *         high       : high bound (inclusive) of this node's range.
 *         queryLow   : low bound (inclusive) of the query range.
 *         queryHigh  : high bound (inclusive) of the query range.
 *
 * Returns:
 *         Sum of array elements within [queryLow, queryHigh].
 *****************************************************************************/
int querySegmentTree(int treeIndex, int low, int high, int queryLow, int queryHigh)
{
    int mid;

    if ((queryHigh < low) || (high < queryLow))
    {
        return 0;
    }
    if ((queryLow <= low) && (high <= queryHigh))
    {
        return tree[treeIndex];
    }

    mid = low + (high - low) / 2;
    return querySegmentTree(2 * treeIndex + 1, low, mid, queryLow, queryHigh) +
           querySegmentTree(2 * treeIndex + 2, mid + 1, high, queryLow, queryHigh);
}

int main(void)
{
    int arr[ARRAY_SIZE] = { 1, 3, 5, 7, 9, 11, 13, 15 };

    buildSegmentTree(arr, 0, 0, ARRAY_SIZE - 1);
    printf("sum[1..5] = %d\n", querySegmentTree(0, 0, ARRAY_SIZE - 1, 1, 5));

    updateSegmentTree(0, 0, ARRAY_SIZE - 1, 3, 100);
    printf("sum[1..5] after update = %d\n", querySegmentTree(0, 0, ARRAY_SIZE - 1, 1, 5));

    return 0;
}
