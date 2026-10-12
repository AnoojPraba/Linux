// Cocktail shaker sort (bidirectional bubble sort) and gnome sort, the simple
// O(n^2) sorts that exploit presorted data.

#include <stdio.h>

static void show(const char *label, const int *a, int n)
{
    printf("%-14s:", label);
    for (int i = 0; i < n; i++) printf(" %d", a[i]);
    printf("\n");
}

static void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

static void cocktail(int *a, int n)
{
    int lo = 0, hi = n - 1, swapped = 1;
    while (swapped)
    {
        swapped = 0;
        for (int i = lo; i < hi; i++)                      /* forward pass floats the maximum to the right */
            if (a[i] > a[i + 1]) { swap(&a[i], &a[i + 1]); swapped = 1; }
        hi--;
        for (int i = hi; i > lo; i--)                      /* backward pass sinks the minimum to the left */
            if (a[i - 1] > a[i]) { swap(&a[i - 1], &a[i]); swapped = 1; }
        lo++;
    }
}

static void gnome(int *a, int n)
{
    for (int i = 0; i < n;)
        if (i == 0 || a[i - 1] <= a[i]) i++;
        else { swap(&a[i], &a[i - 1]); i--; }              /* step back after a swap, forward otherwise */
}

int main(void)
{
    int a[] = {5, 1, 4, 2, 8, 0, 2}, b[] = {5, 1, 4, 2, 8, 0, 2};
    cocktail(a, 7);
    gnome(b, 7);
    show("cocktail", a, 7);
    show("gnome", b, 7);
    int t[] = {2, 3, 4, 5, 1};                            /* the small element at the end: cocktail needs one backward pass */
    cocktail(t, 5);
    show("one-pass case", t, 5);
    return 0;
}
