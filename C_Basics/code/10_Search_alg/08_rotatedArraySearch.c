// Searching a sorted array that was rotated: find the rotation point (minimum)
// and search for a value in O(log n), including duplicates.

#include <stdio.h>

static int find_min(const int *a, int n)
{
    int lo = 0, hi = n - 1;
    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] > a[hi]) lo = mid + 1;                 /* the minimum is to the right of mid */
        else if (a[mid] < a[hi]) hi = mid;
        else hi--;                                        /* equal values (duplicates): cannot decide, shrink by one */
    }
    return lo;
}

static int search(const int *a, int n, int key)           /* distinct values */
{
    int lo = 0, hi = n - 1;
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == key) return mid;
        if (a[lo] <= a[mid])                              /* the left half is sorted */
        {
            if (a[lo] <= key && key < a[mid]) hi = mid - 1; else lo = mid + 1;
        }
        else                                              /* the right half is sorted */
        {
            if (a[mid] < key && key <= a[hi]) lo = mid + 1; else hi = mid - 1;
        }
    }
    return -1;
}

int main(void)
{
    int a[] = {15, 18, 2, 3, 6, 12};
    printf("rotation point (index of the minimum): %d\n", find_min(a, 6));
    int keys[] = {3, 15, 12, 7};
    for (int i = 0; i < 4; i++) printf("search %2d -> %d\n", keys[i], search(a, 6, keys[i]));
    int d[] = {2, 5, 6, 0, 0, 1, 2};
    printf("with duplicates the minimum is at index %d (worst case O(n))\n", find_min(d, 7));
    int sorted[] = {1, 2, 3, 4, 5};
    printf("not rotated at all: minimum index %d\n", find_min(sorted, 5));
    return 0;
}
