// Shell sort: insertion sort over shrinking gaps (Knuth and halving sequences)
// with the comparison counts of both.

#include <stdio.h>
#include <string.h>

static long shell(int *a, int n, int knuth)
{
    long cmp = 0;
    int gap = 1;
    if (knuth) while (gap < n / 3) gap = gap * 3 + 1;     /* 1, 4, 13, 40, ... */
    else gap = n / 2;
    for (; gap > 0; gap = knuth ? (gap - 1) / 3 : gap / 2)
        for (int i = gap; i < n; i++)                      /* a gapped insertion sort */
        {
            int v = a[i], j = i;
            while (j >= gap && (++cmp, a[j - gap] > v))
            {
                a[j] = a[j - gap];
                j -= gap;
            }
            a[j] = v;
        }
    return cmp;
}

int main(void)
{
    int base[40], a[40], b[40];
    unsigned s = 12345;
    for (int i = 0; i < 40; i++) { s = s * 1103515245u + 12345u; base[i] = (int)(s >> 16) % 100; }
    memcpy(a, base, sizeof base);
    memcpy(b, base, sizeof base);
    long c1 = shell(a, 40, 1), c2 = shell(b, 40, 0);
    printf("sorted:");
    for (int i = 0; i < 10; i++) printf(" %d", a[i]);
    printf(" ...\nKnuth gaps: %ld comparisons, halving gaps: %ld comparisons\n", c1, c2);
    int same = memcmp(a, b, sizeof a) == 0;
    printf("both give the same result: %s\n", same ? "yes" : "no");
    return 0;
}
