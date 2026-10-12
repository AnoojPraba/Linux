// Longest common prefix of several strings by vertical scan, sorting (compare
// first and last) and divide and conquer.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int vertical(const char **s, int n)
{
    for (int i = 0;; i++)
        for (int k = 0; k < n; k++)
            if (!s[k][i] || s[k][i] != s[0][i]) return i;   /* first column where some word differs or ends */
}

static int cmp(const void *a, const void *b) { return strcmp(*(const char **)a, *(const char **)b); }

static int by_sorting(const char **s, int n)
{
    qsort(s, n, sizeof s[0], cmp);                          /* after sorting only the first and last can differ most */
    int i = 0;
    while (s[0][i] && s[0][i] == s[n - 1][i]) i++;
    return i;
}

static int common(const char *a, const char *b)
{
    int i = 0;
    while (a[i] && a[i] == b[i]) i++;
    return i;
}

int main(void)
{
    const char *words[] = {"flower", "flow", "flight", "flap"};
    printf("vertical scan : %.*s\n", vertical(words, 4), words[0]);
    const char *w2[] = {"interview", "internet", "interval", "internal"};
    int len = by_sorting(w2, 4);
    printf("sorting       : %.*s\n", len, w2[0]);
    const char *w3[] = {"dog", "racecar", "car"};
    printf("no common prefix: %d characters\n", vertical(w3, 3));
    printf("pairwise helper: %d\n", common("abcdef", "abcxyz"));
    return 0;
}
