// A bit array (one bit per number): sieve of Eratosthenes with 8 numbers per
// byte, plus set/clear/test macros.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SET(a, i)   ((a)[(i) >> 3] |= (unsigned char)(1u << ((i) & 7)))
#define CLEAR(a, i) ((a)[(i) >> 3] &= (unsigned char)~(1u << ((i) & 7)))
#define TEST(a, i)  ((a)[(i) >> 3] >> ((i) & 7) & 1)

int main(void)
{
    const int N = 100000;
    unsigned char *composite = calloc(N / 8 + 1, 1);     /* 12.5 KB instead of 100 KB for a byte per number */
    int count = 0;
    for (int i = 2; i <= N; i++)
    {
        if (TEST(composite, i)) continue;
        count++;
        for (long j = (long)i * i; j <= N; j += i) SET(composite, j);   /* start at i*i: smaller multiples were already marked */
    }
    printf("primes up to %d: %d\n", N, count);
    printf("memory used: %d bytes for %d numbers\n", N / 8 + 1, N);

    printf("primes below 50:");
    for (int i = 2; i < 50; i++)
        if (!TEST(composite, i)) printf(" %d", i);
    printf("\n");

    unsigned char flags[2] = {0, 0};
    SET(flags, 3); SET(flags, 9); SET(flags, 15); CLEAR(flags, 9);
    printf("flags: bit3=%d bit9=%d bit15=%d\n", TEST(flags, 3), TEST(flags, 9), TEST(flags, 15));
    free(composite);
    return 0;
}
