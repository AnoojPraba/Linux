#include <stdio.h>

// Naive recursion: O(2^n) - each call branches into two more, recomputing
// the same sub-values many times.
int fibNaive(int n)
{
    if (n <= 1)
    {
        return n;
    }
    return fibNaive(n - 1) + fibNaive(n - 2);
}

// Memoized recursion: O(n) - each value is computed once and cached.
#define MAX_FIB_N 40

int fibMemo(int n, int *cache)
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

int main()
{
    int n = 10;
    int cache[MAX_FIB_N];
    int i;

    printf("fibNaive(%d) = %d\n", n, fibNaive(n));

    for (i = 0; i < MAX_FIB_N; i++)
    {
        cache[i] = -1;
    }
    printf("fibMemo(%d) = %d\n", n, fibMemo(n, cache));

    return 0;
}
