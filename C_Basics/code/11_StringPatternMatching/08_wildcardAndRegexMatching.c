// Pattern matching with DP: wildcards ('?' and '*') and a small regular-
// expression matcher ('.' and '*' as repetition).

#include <stdio.h>
#include <string.h>

/* wildcard: '?' = any one character, '*' = any sequence (including empty) */
static int wildcard(const char *s, const char *p)
{
    int n = (int)strlen(s), m = (int)strlen(p);
    static int dp[32][32];
    memset(dp, 0, sizeof dp);
    dp[0][0] = 1;
    for (int j = 1; j <= m; j++)
        if (p[j - 1] == '*') dp[0][j] = dp[0][j - 1];     /* a leading star may match the empty string */
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
        {
            if (p[j - 1] == '*') dp[i][j] = dp[i][j - 1] || dp[i - 1][j];       /* empty, or consume one more character */
            else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) dp[i][j] = dp[i - 1][j - 1];
        }
    return dp[n][m];
}

/* regex: '.' = any one character, 'x*' = zero or more x */
static int regex(const char *s, const char *p)
{
    if (!*p) return !*s;
    int first = *s && (*p == *s || *p == '.');
    if (p[1] == '*')
        return regex(s, p + 2) || (first && regex(s + 1, p));   /* zero uses, or one use and stay on the star */
    return first && regex(s + 1, p + 1);
}

int main(void)
{
    const char *w[][2] = {{"adceb", "*a*b"}, {"acdcb", "a*c?b"}, {"cb", "?a"}, {"", "***"}};
    for (int i = 0; i < 4; i++)
        printf("wildcard \"%s\" vs \"%s\": %s\n", w[i][0], w[i][1], wildcard(w[i][0], w[i][1]) ? "match" : "no match");
    const char *r[][2] = {{"aa", "a"}, {"aa", "a*"}, {"ab", ".*"}, {"aab", "c*a*b"}, {"mississippi", "mis*is*p*."}};
    for (int i = 0; i < 5; i++)
        printf("regex    \"%s\" vs \"%s\": %s\n", r[i][0], r[i][1], regex(r[i][0], r[i][1]) ? "match" : "no match");
    return 0;
}
