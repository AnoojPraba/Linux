// The binary search family on sorted arrays: lower/upper bound, first and last
// occurrence, count of a value, insert position and floor/ceiling.

#include <stdio.h>

/* first index with a[i] >= key (n if none) */
static int lower_bound(const int *a, int n, int key)
{
    int lo = 0, hi = n;                                  /* the answer lies in [lo, hi] */
    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;                    /* no overflow */
        if (a[mid] < key) lo = mid + 1; else hi = mid;
    }
    return lo;
}

/* first index with a[i] > key */
static int upper_bound(const int *a, int n, int key)
{
    int lo = 0, hi = n;
    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] <= key) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static int binary_search_recursive(const int *a, int lo, int hi, int key)
{
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;
    if (a[mid] == key) return mid;
    return a[mid] < key ? binary_search_recursive(a, mid + 1, hi, key) : binary_search_recursive(a, lo, mid - 1, key);
}

int main(void)
{
    int a[] = {1, 2, 4, 4, 4, 4, 7, 9, 12};
    int n = 9;
    int lb = lower_bound(a, n, 4), ub = upper_bound(a, n, 4);
    printf("4 occurs at indexes %d..%d (count %d)\n", lb, ub - 1, ub - lb);
    printf("insert position of 5: %d, of 0: %d, of 100: %d\n", lower_bound(a, n, 5), lower_bound(a, n, 0), lower_bound(a, n, 100));

    int fl = upper_bound(a, n, 8) - 1;                   /* floor: largest element <= 8 */
    int ce = lower_bound(a, n, 8);                       /* ceiling: smallest element >= 8 */
    printf("floor(8) = %d, ceiling(8) = %d\n", a[fl], a[ce]);
    printf("recursive search for 9 -> index %d, for 5 -> %d\n", binary_search_recursive(a, 0, n - 1, 9), binary_search_recursive(a, 0, n - 1, 5));

    int lo = 2000000000, hi = 2100000000;
    printf("(lo + hi)/2 overflows int; lo + (hi - lo)/2 = %d\n", lo + (hi - lo) / 2);
    return 0;
}
