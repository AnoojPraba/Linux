// Pancake sort: sort using only prefix reversals (flips), recording the flip
// sequence.

#include <stdio.h>

static void show(const char *label, const int *a, int n)
{
    printf("%-14s:", label);
    for (int i = 0; i < n; i++) printf(" %d", a[i]);
    printf("\n");
}

static void flip(int *a, int k)
{
    for (int i = 0, j = k; i < j; i++, j--) { int t = a[i]; a[i] = a[j]; a[j] = t; }
}

int main(void)
{
    int a[] = {3, 6, 1, 9, 4, 8, 2}, n = 7, flips = 0;
    show("start", a, n);
    for (int size = n; size > 1; size--)
    {
        int max = 0;
        for (int i = 1; i < size; i++)
            if (a[i] > a[max]) max = i;
        if (max == size - 1) continue;                     /* the largest of the unsorted part is already at its end */
        if (max > 0) { flip(a, max); flips++; printf("flip(%d)\n", max + 1); }   /* bring it to the front */
        flip(a, size - 1); flips++;                        /* then flip it into place */
        printf("flip(%d)\n", size);
    }
    show("sorted", a, n);
    printf("%d flips (at most 2n - 3 = %d)\n", flips, 2 * n - 3);
    return 0;
}
