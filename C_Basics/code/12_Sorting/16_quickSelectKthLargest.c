// Quickselect: the k-th smallest/largest element in expected O(n) without
// sorting, plus the median.

#include <stdio.h>
#include <string.h>

static void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

static int partition(int *a, int lo, int hi)
{
    int pivot = a[hi], i = lo;
    for (int j = lo; j < hi; j++)
        if (a[j] < pivot) swap(&a[i++], &a[j]);
    swap(&a[i], &a[hi]);
    return i;
}

static int quickselect(int *a, int n, int k)              /* k-th smallest, 0-based */
{
    int lo = 0, hi = n - 1;
    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;
        swap(&a[mid], &a[hi]);                             /* the middle element as the pivot defends against sorted input */
        int p = partition(a, lo, hi);
        if (p == k) return a[p];
        if (p < k) lo = p + 1; else hi = p - 1;            /* only the side that contains k is processed */
    }
    return a[lo];
}

int main(void)
{
    int orig[] = {7, 10, 4, 3, 20, 15, 8, 1, 12};
    int n = 9, a[9];
    memcpy(a, orig, sizeof orig);
    printf("3rd smallest = %d\n", quickselect(a, n, 2));
    memcpy(a, orig, sizeof orig);
    printf("2nd largest  = %d\n", quickselect(a, n, n - 2));
    memcpy(a, orig, sizeof orig);
    printf("median       = %d\n", quickselect(a, n, n / 2));
    return 0;
}
