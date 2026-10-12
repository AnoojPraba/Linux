// Suffix array (sorted suffixes) and Kasai's LCP array: longest repeated
// substring and distinct-substring count.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *S;

static int cmp_suffix(const void *a, const void *b)
{
    return strcmp(S + *(const int *)a, S + *(const int *)b);
}

int main(void)
{
    S = "banana";
    int n = (int)strlen(S), sa[16], rank_[16], lcp[16];
    for (int i = 0; i < n; i++) sa[i] = i;
    qsort(sa, n, sizeof sa[0], cmp_suffix);              /* simple O(n^2 log n) construction; doubling gives O(n log n) */
    for (int i = 0; i < n; i++) rank_[sa[i]] = i;

    printf("suffix array of %s:\n", S);
    for (int i = 0; i < n; i++) printf("  %d: %s\n", sa[i], S + sa[i]);

    /* Kasai: lcp[i] = common prefix length of suffixes sa[i-1] and sa[i], computed in O(n) */
    lcp[0] = 0;
    for (int i = 0, h = 0; i < n; i++)
    {
        if (rank_[i] > 0)
        {
            int j = sa[rank_[i] - 1];
            while (i + h < n && j + h < n && S[i + h] == S[j + h]) h++;
            lcp[rank_[i]] = h;
            if (h > 0) h--;                              /* the next suffix loses at most one matching character */
        }
        else
            h = 0;
    }
    int best = 0, at = 0, distinct = n * (n + 1) / 2;
    for (int i = 1; i < n; i++)
    {
        distinct -= lcp[i];                              /* shared prefixes are not new substrings */
        if (lcp[i] > best) { best = lcp[i]; at = sa[i]; }
    }
    printf("LCP array:");
    for (int i = 0; i < n; i++) printf(" %d", lcp[i]);
    printf("\nlongest repeated substring: %.*s (length %d)\n", best, S + at, best);
    printf("number of distinct substrings: %d\n", distinct);
    return 0;
}
