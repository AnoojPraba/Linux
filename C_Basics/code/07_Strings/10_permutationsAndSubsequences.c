// Generating strings: all permutations (with and without duplicates), all
// subsequences with bitmasks, and the next lexicographic permutation.

#include <stdio.h>
#include <string.h>

static int count;

static void swap(char *a, char *b) { char t = *a; *a = *b; *b = t; }

static void permute(char *s, int k, int n)
{
    if (k == n)
    {
        if (count++ < 6) printf("  %s\n", s);
        return;
    }
    for (int i = k; i < n; i++)
    {
        int seen = 0;
        for (int j = k; j < i; j++)
            if (s[j] == s[i]) seen = 1;                 /* skip a character already tried at this position (duplicates) */
        if (seen) continue;
        swap(&s[k], &s[i]);
        permute(s, k + 1, n);
        swap(&s[k], &s[i]);                             /* backtrack */
    }
}

static int next_permutation(char *s, int n)
{
    int i = n - 2;
    while (i >= 0 && s[i] >= s[i + 1]) i--;             /* the rightmost ascent */
    if (i < 0) return 0;                                /* already the last permutation */
    int j = n - 1;
    while (s[j] <= s[i]) j--;
    swap(&s[i], &s[j]);
    for (int l = i + 1, r = n - 1; l < r; l++, r--) swap(&s[l], &s[r]);   /* the suffix becomes the smallest */
    return 1;
}

int main(void)
{
    char s[] = "aab";
    printf("distinct permutations of aab:\n");
    permute(s, 0, 3);
    printf("total %d\n", count);

    const char *t = "abc";
    printf("all subsequences of abc:");
    for (int mask = 0; mask < (1 << 3); mask++)
    {
        printf(" \"");
        for (int i = 0; i < 3; i++)
            if (mask >> i & 1) putchar(t[i]);
        printf("\"");
    }
    printf("\n");

    char p[] = "1243";
    next_permutation(p, 4);
    printf("next permutation after 1243 is %s\n", p);
    char last[] = "321";
    printf("next permutation after 321 exists: %d\n", next_permutation(last, 3));
    return 0;
}
