// Fast exponentiation mod m, inversion counting inside merge sort, and the
// master-theorem examples.

#include <stdio.h>
#include <string.h>

/* x^n mod m in O(log n): halve the exponent each time */
static unsigned long long power_mod(unsigned long long x, unsigned long long n, unsigned long long m)
{
    if (n == 0) return 1 % m;
    unsigned long long half = power_mod(x, n / 2, m);
    unsigned long long r = half * half % m;
    return (n & 1) ? r * (x % m) % m : r;
}

/* Count inversions (pairs i<j with a[i]>a[j]) while merge-sorting a[lo..hi). */
static long merge_count(int *a, int *tmp, int lo, int hi)
{
    if (hi - lo < 2) return 0;
    int mid = lo + (hi - lo) / 2;
    long inv = merge_count(a, tmp, lo, mid) + merge_count(a, tmp, mid, hi);   /* divide, conquer */

    int i = lo, j = mid, k = lo;                                              /* combine */
    while (i < mid && j < hi)
    {
        if (a[i] <= a[j])
            tmp[k++] = a[i++];
        else
        {
            tmp[k++] = a[j++];
            inv += mid - i;                    /* a[j] jumps over every remaining left element */
        }
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi)  tmp[k++] = a[j++];
    memcpy(a + lo, tmp + lo, (size_t)(hi - lo) * sizeof *a);
    return inv;
}

int main(void)
{
    printf("2^10 = %llu\n", power_mod(2, 10, 1000000007ULL));
    printf("3^200 mod 1000000007 = %llu\n", power_mod(3, 200, 1000000007ULL));

    int a[] = {8, 4, 2, 1, 7, 3};
    int tmp[6];
    long inv = merge_count(a, tmp, 0, 6);
    printf("inversions = %ld\n", inv);
    printf("sorted     = %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5]);
    return 0;
}
