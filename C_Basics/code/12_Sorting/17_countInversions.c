// Counting inversions with merge sort in O(n log n), and what the number means
// (how far from sorted).

#include <stdio.h>
#include <string.h>

static long merge_count(int *a, int *tmp, int lo, int hi)  /* sorts a[lo, hi) and returns its inversions */
{
    if (hi - lo < 2) return 0;
    int mid = lo + (hi - lo) / 2;
    long inv = merge_count(a, tmp, lo, mid) + merge_count(a, tmp, mid, hi);
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi)
    {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else { tmp[k++] = a[j++]; inv += mid - i; }        /* a[j] is smaller than every remaining element of the left half */
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    memcpy(a + lo, tmp + lo, (size_t)(hi - lo) * sizeof *a);
    return inv;
}

static long brute(const int *a, int n)
{
    long c = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) c += a[i] > a[j];
    return c;
}

int main(void)
{
    int a[] = {8, 4, 2, 1, 7, 3}, b[6], tmp[6];
    memcpy(b, a, sizeof a);
    printf("inversions (O(n^2) check): %ld\n", brute(a, 6));
    printf("inversions (merge sort)  : %ld\n", merge_count(b, tmp, 0, 6));
    int sorted[] = {1, 2, 3, 4}, rev[] = {4, 3, 2, 1}, t2[4];
    printf("sorted: %ld, reversed (n(n-1)/2 = 6): %ld\n", merge_count(sorted, t2, 0, 4), merge_count(rev, t2, 0, 4));
    return 0;
}
