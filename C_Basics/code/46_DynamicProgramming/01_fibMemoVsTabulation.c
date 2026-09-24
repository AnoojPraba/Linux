#include <stdio.h>

#define MAX_N 40

// Top-down / memoization: still recursive, but a cache stops the same
// sub-problem from being recomputed - contrast with the naive O(2^n)
// recursion in 14_Recursion/02_fibonacci.c, which this file's cache-based
// version mirrors.
int fibMemo(int n, int cache[])
{
    if (n <= 1)
    {
        return n;
    }
    if (cache[n] != -1)
    {
        return cache[n];
    }
    cache[n] = fibMemo(n - 1, cache) + fibMemo(n - 2, cache);
    return cache[n];
}

// Bottom-up / tabulation: no recursion at all - build the table forward
// from the base cases up to n. Same O(n) time as memoization, but avoids
// call-stack depth entirely, which matters for large n.
int fibTabulation(int n)
{
    int table[MAX_N];
    int i;

    table[0] = 0;
    table[1] = 1;
    for (i = 2; i <= n; i++)
    {
        table[i] = table[i - 1] + table[i - 2];
    }
    return table[n];
}

int main()
{
    int n = 15;
    int cache[MAX_N];
    int i;

    for (i = 0; i < MAX_N; i++)
    {
        cache[i] = -1;
    }

    printf("fibMemo(%d) = %d\n", n, fibMemo(n, cache));
    printf("fibTabulation(%d) = %d\n", n, fibTabulation(n));

    return 0;
}
