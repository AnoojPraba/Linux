// Natural merge sort: detect the existing ascending runs and merge them
// pairwise, the idea behind Timsort (adaptive, O(n) on sorted input).

#include <stdio.h>
#include <string.h>

static void show(const char *label, const int *a, int n)
{
    printf("%-14s:", label);
    for (int i = 0; i < n; i++) printf(" %d", a[i]);
    printf("\n");
}

static int runs(const int *a, int n, int *start)
{
    int r = 0;
    for (int i = 0; i < n;)
    {
        start[r++] = i;
        while (i + 1 < n && a[i] <= a[i + 1]) i++;         /* extend the ascending run */
        i++;
    }
    start[r] = n;
    return r;
}

static void merge(int *a, int lo, int mid, int hi, int *tmp)
{
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) tmp[k++] = a[i] <= a[j] ? a[i++] : a[j++];   /* <= keeps the sort stable */
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    memcpy(a + lo, tmp + lo, (size_t)(hi - lo) * sizeof *a);
}

int main(void)
{
    int a[] = {1, 4, 9, 2, 3, 8, 10, 5, 6, 7, 0};
    int n = 11, start[16], tmp[16], passes = 0;
    int r = runs(a, n, start);
    printf("found %d natural runs\n", r);
    while (r > 1)
    {
        int out = 0;
        for (int i = 0; i < r; i += 2)
        {
            if (i + 1 < r) merge(a, start[i], start[i + 1], start[i + 2], tmp);
            start[out++] = start[i];                       /* two runs became one */
        }
        start[out] = n;
        r = out;
        passes++;
    }
    show("sorted", a, n);
    printf("merge passes: %d (a plain bottom-up merge sort needs ceil(log2 n) = 4)\n", passes);
    int sorted[] = {1, 2, 3, 4, 5};
    printf("already sorted input has %d run(s): O(n) work\n", runs(sorted, 5, start));
    return 0;
}
