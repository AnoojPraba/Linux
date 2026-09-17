#include <stdio.h>
#include <string.h>

#define MAX_LEN 32

int max(int a, int b)
{
    return (a > b) ? a : b;
}

// LCS: the longest sequence of characters appearing in both strings, in
// order but not necessarily contiguous. table[i][j] = LCS length using the
// first i characters of a and the first j characters of b - matching
// characters extend the diagonal's LCS by one; a mismatch takes the best
// of dropping one character from either string.
int longestCommonSubsequence(const char *a, const char *b)
{
    int lenA = strlen(a);
    int lenB = strlen(b);
    int table[MAX_LEN + 1][MAX_LEN + 1];
    int i;
    int j;

    for (i = 0; i <= lenA; i++)
    {
        for (j = 0; j <= lenB; j++)
        {
            if ((i == 0) || (j == 0))
            {
                table[i][j] = 0;
            }
            else if (a[i - 1] == b[j - 1])
            {
                table[i][j] = table[i - 1][j - 1] + 1;
            }
            else
            {
                table[i][j] = max(table[i - 1][j], table[i][j - 1]);
            }
        }
    }

    return table[lenA][lenB];
}

int main()
{
    const char *a = "ABCBDAB";
    const char *b = "BDCABA";

    printf("LCS(\"%s\", \"%s\") length = %d\n", a, b,
           longestCommonSubsequence(a, b));

    return 0;
}
