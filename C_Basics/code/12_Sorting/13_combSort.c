// Comb sort: bubble sort with a shrinking gap (factor 1.3) that removes
// 'turtles' (small values near the end).

#include <stdio.h>

static void show(const char *label, const int *a, int n)
{
    printf("%-14s:", label);
    for (int i = 0; i < n; i++) printf(" %d", a[i]);
    printf("\n");
}

int main(void)
{
    int a[] = {8, 4, 1, 56, 3, -44, 23, -6, 28, 0};
    int n = 10, gap = n, swapped = 1;
    while (gap > 1 || swapped)
    {
        gap = gap * 10 / 13;                               /* shrink factor about 1.3 */
        if (gap < 1) gap = 1;
        swapped = 0;
        for (int i = 0; i + gap < n; i++)
            if (a[i] > a[i + gap])
            {
                int t = a[i]; a[i] = a[i + gap]; a[i + gap] = t;
                swapped = 1;
            }
    }
    show("comb sorted", a, n);
    return 0;
}
