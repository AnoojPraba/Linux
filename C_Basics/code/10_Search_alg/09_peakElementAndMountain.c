// Binary search without a target: find a peak element, the peak of a mountain
// array and the single element in a sorted array of pairs.

#include <stdio.h>

static int peak(const int *a, int n)                      /* any index i with a[i] greater than its neighbours */
{
    int lo = 0, hi = n - 1;
    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] < a[mid + 1]) lo = mid + 1;            /* uphill: a peak exists to the right */
        else hi = mid;                                    /* downhill or flat top: a peak is at mid or to the left */
    }
    return lo;
}

static int single_in_pairs(const int *a, int n)           /* sorted, every value twice except one */
{
    int lo = 0, hi = n - 1;
    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (mid % 2) mid--;                               /* look at the first element of a pair slot */
        if (a[mid] == a[mid + 1]) lo = mid + 2;           /* pairs intact so far: the single is to the right */
        else hi = mid;
    }
    return a[lo];
}

int main(void)
{
    int mountain[] = {1, 3, 8, 12, 9, 4, 2};
    printf("mountain peak at index %d (value %d)\n", peak(mountain, 7), mountain[peak(mountain, 7)]);
    int bumpy[] = {1, 2, 1, 3, 5, 6, 4};
    printf("a peak of the bumpy array: index %d (value %d); indexes 1 and 5 are both valid peaks\n", peak(bumpy, 7), bumpy[peak(bumpy, 7)]);
    int pairs[] = {1, 1, 2, 3, 3, 4, 4, 8, 8};
    printf("single element in the sorted pairs: %d\n", single_in_pairs(pairs, 9));
    return 0;
}
