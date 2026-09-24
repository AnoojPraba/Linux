#include <stdio.h>
#include <string.h>

#define MAX_PATTERN_LEN 128

// Build the "longest proper prefix that is also a suffix" (LPS) array: for
// each prefix of the pattern, lps[i] is the length of the longest proper
// prefix that also occurs as a suffix ending at i. This is what lets the
// search skip re-comparing characters it already knows match on a mismatch.
void buildLps(const char *pattern, int m, int lps[])
{
    int len = 0;
    int i = 1;

    lps[0] = 0;
    while (i < m)
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else if (len != 0)
        {
            len = lps[len - 1];
        }
        else
        {
            lps[i] = 0;
            i++;
        }
    }
}

// KMP: on a mismatch, use the LPS array to jump the pattern pointer forward
// instead of restarting - the text pointer "i" never moves backward, giving
// O(n + m) total time regardless of how repetitive the pattern is.
void kmpSearch(const char *text, const char *pattern)
{
    int n = (int) strlen(text);
    int m = (int) strlen(pattern);
    int lps[MAX_PATTERN_LEN];
    int i = 0;
    int j = 0;

    buildLps(pattern, m, lps);

    while (i < n)
    {
        if (text[i] == pattern[j])
        {
            i++;
            j++;
            if (j == m)
            {
                printf("Match at index %d\n", i - j);
                j = lps[j - 1];
            }
        }
        else if (j != 0)
        {
            j = lps[j - 1];
        }
        else
        {
            i++;
        }
    }
}

int main()
{
    const char *text = "abxabcabcaby";
    const char *pattern = "abcaby";

    kmpSearch(text, pattern);

    return 0;
}
