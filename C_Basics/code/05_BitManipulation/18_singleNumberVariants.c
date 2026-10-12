// XOR and counting tricks for finding unique elements: one single, two
// singles, one single among triples, and the missing/duplicate pair.

#include <stdio.h>

int main(void)
{
    int a[] = {4, 1, 2, 1, 2};
    int one = 0;
    for (int i = 0; i < 5; i++) one ^= a[i];             /* pairs cancel, the single number remains */
    printf("single number among pairs: %d\n", one);

    int b[] = {1, 2, 1, 3, 2, 5};                        /* two singles: 3 and 5 */
    int x = 0;
    for (int i = 0; i < 6; i++) x ^= b[i];               /* x = 3 ^ 5 */
    int low = x & -x, first = 0, second = 0;             /* a bit where the two singles differ splits the array in two groups */
    for (int i = 0; i < 6; i++)
        if (b[i] & low) first ^= b[i]; else second ^= b[i];
    printf("two singles: %d and %d\n", first, second);

    int c[] = {2, 2, 3, 2, 7, 7, 7, 9, 9, 9, 5, 5, 5};    /* every number three times except 3 */
    int ones = 0, twos = 0;                              /* bit counters modulo 3 */
    for (int i = 0; i < 13; i++)
    {
        ones = (ones ^ c[i]) & ~twos;
        twos = (twos ^ c[i]) & ~ones;
    }
    printf("single number among triples: %d\n", ones);

    int d[] = {1, 2, 2, 4};                              /* 1..4 with 2 duplicated and 3 missing */
    int xr = 0;
    for (int i = 0; i < 4; i++) xr ^= d[i] ^ (i + 1);    /* duplicate ^ missing */
    int bit = xr & -xr, g1 = 0, g2 = 0;
    for (int i = 0; i < 4; i++)
    {
        if (d[i] & bit) g1 ^= d[i]; else g2 ^= d[i];
        if ((i + 1) & bit) g1 ^= i + 1; else g2 ^= i + 1;
    }
    int dup = 0;
    for (int i = 0; i < 4; i++) if (d[i] == g1) dup = 1;
    printf("duplicate = %d, missing = %d\n", dup ? g1 : g2, dup ? g2 : g1);
    return 0;
}
