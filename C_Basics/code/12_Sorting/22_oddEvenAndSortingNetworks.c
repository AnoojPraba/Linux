// Odd-even transposition sort (parallel-friendly) and a bitonic sorting
// network for 8 elements: fixed compare-exchange schedules.

#include <stdio.h>

static void show(const char *label, const int *a, int n)
{
    printf("%-14s:", label);
    for (int i = 0; i < n; i++) printf(" %d", a[i]);
    printf("\n");
}

static void cx(int *a, int i, int j, int up)             /* compare-exchange: order a[i], a[j] ascending if up */
{
    if ((a[i] > a[j]) == up) { int t = a[i]; a[i] = a[j]; a[j] = t; }
}

static void odd_even(int *a, int n)
{
    for (int round = 0; round < n; round++)               /* all compares inside one round are independent */
        for (int i = round % 2; i + 1 < n; i += 2) cx(a, i, i + 1, 1);
}

static void bitonic_merge(int *a, int lo, int n, int up)
{
    if (n <= 1) return;
    int m = n / 2;
    for (int i = lo; i < lo + m; i++) cx(a, i, i + m, up);
    bitonic_merge(a, lo, m, up);
    bitonic_merge(a, lo + m, m, up);
}

static void bitonic_sort(int *a, int lo, int n, int up)  /* n must be a power of two */
{
    if (n <= 1) return;
    int m = n / 2;
    bitonic_sort(a, lo, m, 1);                             /* ascending half then descending half = a bitonic sequence */
    bitonic_sort(a, lo + m, m, 0);
    bitonic_merge(a, lo, n, up);
}

int main(void)
{
    int a[] = {9, 3, 7, 1, 8, 2, 6, 4}, b[] = {9, 3, 7, 1, 8, 2, 6, 4};
    odd_even(a, 8);
    bitonic_sort(b, 0, 8, 1);
    show("odd-even", a, 8);
    show("bitonic", b, 8);
    return 0;
}
