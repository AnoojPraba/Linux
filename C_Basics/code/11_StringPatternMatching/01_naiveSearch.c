#include <stdio.h>
#include <string.h>

// Brute force: try every starting offset in the text and compare the
// pattern byte-by-byte. Worst case O(n*m): e.g. text "aaaa...a" and pattern
// "aaab" - every position matches m-1 characters before finally failing.
void naiveSearch(const char *text, const char *pattern)
{
    int n = (int) strlen(text);
    int m = (int) strlen(pattern);
    int i;

    for (i = 0; i <= n - m; i++)
    {
        int j = 0;

        while ((j < m) && (text[i + j] == pattern[j]))
        {
            j++;
        }
        if (j == m)
        {
            printf("Match at index %d\n", i);
        }
    }
}

int main()
{
    const char *text = "aaaaaaaaaaaaaaaaaab";
    const char *pattern = "aaab";

    naiveSearch(text, pattern);

    return 0;
}
