// Boyer-Moore-Horspool: compare the pattern right to left and skip ahead with
// a bad-character table; sublinear on typical text.

#include <stdio.h>
#include <string.h>

static int horspool(const char *text, const char *pat, int *comparisons)
{
    int n = (int)strlen(text), m = (int)strlen(pat), shift[256];
    for (int i = 0; i < 256; i++) shift[i] = m;          /* a character not in the pattern skips the whole pattern */
    for (int i = 0; i < m - 1; i++) shift[(unsigned char)pat[i]] = m - 1 - i;
    *comparisons = 0;
    for (int pos = 0; pos + m <= n;)
    {
        int j = m - 1;
        while (j >= 0 && (++*comparisons, text[pos + j] == pat[j])) j--;
        if (j < 0) return pos;
        pos += shift[(unsigned char)text[pos + m - 1]];  /* shift by the table entry of the text char under the last pattern char */
    }
    return -1;
}

int main(void)
{
    const char *text = "the quick brown fox jumps over the lazy dog and keeps running far away";
    const char *pats[] = {"lazy", "running", "zebra"};
    for (int i = 0; i < 3; i++)
    {
        int cmp;
        int at = horspool(text, pats[i], &cmp);
        printf("%-8s -> index %2d after %2d comparisons (text length %zu)\n", pats[i], at, cmp, strlen(text));
    }
    return 0;
}
