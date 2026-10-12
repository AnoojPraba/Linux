// Manacher's algorithm: all palindromic substrings and the longest one in
// O(n), compared with expand-around-centre.

#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *s = "babcbabcbaccba";
    int n = (int)strlen(s);

    /* transform: abc -> ^#a#b#c#$ so odd and even palindromes both have a centre */
    char t[64];
    int m = 0;
    t[m++] = '^';
    for (int i = 0; i < n; i++) { t[m++] = '#'; t[m++] = s[i]; }
    t[m++] = '#'; t[m++] = '$'; t[m] = '\0';

    int p[64] = {0}, centre = 0, right = 0;
    for (int i = 1; i < m - 1; i++)
    {
        if (i < right)
        {
            int mirror = 2 * centre - i;                 /* reuse the mirror image inside the current palindrome */
            p[i] = p[mirror] < right - i ? p[mirror] : right - i;
        }
        while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) p[i]++;
        if (i + p[i] > right) { centre = i; right = i + p[i]; }
    }

    int best = 0, at = 0, count = 0;
    for (int i = 1; i < m - 1; i++)
    {
        if (p[i] > best) { best = p[i]; at = i; }
        count += (p[i] + 1) / 2;                         /* palindromes centred here */
    }
    int start = (at - best) / 2;
    printf("longest palindromic substring of %s: %.*s (length %d)\n", s, best, s + start, best);
    printf("number of palindromic substrings: %d\n", count);
    return 0;
}
