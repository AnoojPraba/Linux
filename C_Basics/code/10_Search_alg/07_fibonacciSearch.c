// Fibonacci search: divide a sorted array with Fibonacci numbers using only
// addition and subtraction (useful when division is costly).

#include <stdio.h>

static int fibonacci_search(const int *a, int n, int key, int *probes)
{
    int fib2 = 0, fib1 = 1, fib = fib1 + fib2;            /* fib = smallest Fibonacci number >= n */
    while (fib < n) { fib2 = fib1; fib1 = fib; fib = fib1 + fib2; }
    int offset = -1;                                      /* everything up to offset is eliminated */
    *probes = 0;
    while (fib > 1)
    {
        int i = offset + fib2 < n - 1 ? offset + fib2 : n - 1;
        (*probes)++;
        if (a[i] < key)      { fib = fib1; fib1 = fib2; fib2 = fib - fib1; offset = i; }   /* drop the left 1/3 */
        else if (a[i] > key) { fib = fib2; fib1 = fib1 - fib2; fib2 = fib - fib1; }        /* drop the right 2/3 */
        else return i;
    }
    if (fib1 && offset + 1 < n && a[offset + 1] == key) { (*probes)++; return offset + 1; }
    return -1;
}

int main(void)
{
    int a[] = {10, 22, 35, 40, 45, 50, 80, 82, 85, 90, 100, 235};
    int n = 12, probes;
    int keys[] = {85, 10, 235, 36};
    for (int i = 0; i < 4; i++)
    {
        int at = fibonacci_search(a, n, keys[i], &probes);
        printf("search %3d -> index %2d after %d probes\n", keys[i], at, probes);
    }
    return 0;
}
