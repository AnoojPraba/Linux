// String DP trio: edit distance (Levenshtein), longest common subsequence and
// longest common substring.

#include <stdio.h>
#include <string.h>

#define MAX 32

static int min3(int a, int b, int c) { int m = a < b ? a : b; return m < c ? m : c; }

static int edit_distance(const char *a, const char *b)
{
    int n = (int)strlen(a), m = (int)strlen(b), dp[MAX][MAX];
    for (int i = 0; i <= n; i++) dp[i][0] = i;           /* delete everything */
    for (int j = 0; j <= m; j++) dp[0][j] = j;           /* insert everything */
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dp[i][j] = min3(dp[i - 1][j] + 1,            /* delete a[i-1] */
                            dp[i][j - 1] + 1,            /* insert b[j-1] */
                            dp[i - 1][j - 1] + (a[i - 1] != b[j - 1]));   /* substitute (free if equal) */
    return dp[n][m];
}

static int lcs(const char *a, const char *b, char *out)
{
    int n = (int)strlen(a), m = (int)strlen(b), dp[MAX][MAX] = {{0}};
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dp[i][j] = a[i - 1] == b[j - 1] ? dp[i - 1][j - 1] + 1 : (dp[i - 1][j] > dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1]);
    int k = dp[n][m];
    out[k] = '\0';
    for (int i = n, j = m; i > 0 && j > 0;)              /* walk back to rebuild one LCS */
        if (a[i - 1] == b[j - 1]) { out[--k] = a[i - 1]; i--; j--; }
        else if (dp[i - 1][j] >= dp[i][j - 1]) i--;
        else j--;
    return dp[n][m];
}

static int longest_common_substring(const char *a, const char *b, int *end_a)
{
    int n = (int)strlen(a), m = (int)strlen(b), dp[MAX][MAX] = {{0}}, best = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (a[i - 1] == b[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;          /* the run must be contiguous, so no max with neighbours */
                if (dp[i][j] > best) { best = dp[i][j]; *end_a = i; }
            }
    return best;
}

int main(void)
{
    printf("edit distance kitten -> sitting = %d\n", edit_distance("kitten", "sitting"));
    printf("edit distance sunday -> saturday = %d\n", edit_distance("sunday", "saturday"));
    char out[MAX];
    int len = lcs("AGGTAB", "GXTXAYB", out);
    printf("LCS of AGGTAB and GXTXAYB = %s (length %d)\n", out, len);
    int end;
    int sub = longest_common_substring("zxabcdezy", "yzabcdezx", &end);
    printf("longest common substring = %.*s (length %d)\n", sub, "zxabcdezy" + end - sub, sub);
    return 0;
}
