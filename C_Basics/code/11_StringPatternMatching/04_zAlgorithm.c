// Z-algorithm: the Z-array of a string and linear-time pattern search with the
// pattern#text trick.

#include <stdio.h>
#include <string.h>

/* z[i] = length of the longest substring starting at i that is also a prefix of s */
static void z_function(const char *s, int n, int *z)
{
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; i++)
    {
        z[i] = 0;
        if (i < r)                                      /* inside the current Z-box: reuse earlier work */
            z[i] = z[i - l] < r - i ? z[i - l] : r - i;
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) { l = i; r = i + z[i]; }
    }
}

int main(void)
{
    const char *text = "abxabcabcaby", *pat = "abcaby";
    char s[64];
    snprintf(s, sizeof s, "%s#%s", pat, text);           /* '#' must not occur in either string */
    int n = (int)strlen(s), z[64];
    z_function(s, n, z);

    printf("Z array of %s:", s);
    for (int i = 0; i < n; i++) printf(" %d", z[i]);
    printf("\n");
    int m = (int)strlen(pat);
    for (int i = m + 1; i < n; i++)
        if (z[i] == m)                                  /* a prefix of length m = the pattern matches here */
            printf("pattern found at text index %d\n", i - m - 1);
    return 0;
}
