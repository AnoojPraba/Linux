#include <stdio.h>

long factorialRecursive(int n)
{
    if (n <= 1)
    {
        return 1;
    }
    return n * factorialRecursive(n - 1);
}

long factorialIterative(int n)
{
    long result = 1;
    int i;

    for (i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

int main()
{
    int n = 6;

    printf("factorialRecursive(%d) = %ld\n", n, factorialRecursive(n));
    printf("factorialIterative(%d) = %ld\n", n, factorialIterative(n));

    return 0;
}
