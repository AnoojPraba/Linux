// Enumerating subsets with bitmasks: all subsets, all submasks of a mask,
// subsets of size k (Gosper's hack) and subset-sum counting.

#include <stdio.h>

int main(void)
{
    const char *items[] = {"a", "b", "c", "d"};
    int n = 4;
    printf("all %d subsets:", 1 << n);
    for (int mask = 0; mask < (1 << n); mask++)
    {
        printf(" {");
        for (int i = 0; i < n; i++)
            if (mask >> i & 1) printf("%s", items[i]);
        printf("}");
    }

    unsigned full = 0b1011;                             /* every submask of 1011 (not just numbers below it) */
    printf("\nsubmasks of 1011 in decreasing order:");
    for (unsigned sub = full;; sub = (sub - 1) & full)  /* (sub-1)&full jumps to the next smaller submask */
    {
        printf(" %u", sub);
        if (sub == 0) break;
    }

    printf("\nsubsets of size 2 (Gosper's hack):");
    for (unsigned x = (1u << 2) - 1; x < (1u << n);)
    {
        printf(" %u", x);
        unsigned c = x & -x, r = x + c;                 /* next integer with the same popcount */
        x = (((r ^ x) >> 2) / c) | r;
    }

    int w[] = {3, 34, 4, 12, 5, 2}, target = 9, ways = 0;
    for (int mask = 0; mask < (1 << 6); mask++)
    {
        int sum = 0;
        for (int i = 0; i < 6; i++)
            if (mask >> i & 1) sum += w[i];
        ways += sum == target;
    }
    printf("\nsubsets of {3,34,4,12,5,2} with sum 9: %d\n", ways);

    long total = 0;                                     /* sum over all (mask, submask) pairs = 3^n: enumerate to confirm */
    for (unsigned m = 0; m < (1u << n); m++)
        for (unsigned sub = m;; sub = (sub - 1) & m) { total++; if (!sub) break; }
    printf("pairs (mask, submask) for n = 4: %ld = 3^4\n", total);
    return 0;
}
