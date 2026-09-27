#include <stdio.h>
#include <string.h>

#define MAX_LEN 16

/*****************************************************************************
 * Name: min3
 *
 * Description:
 *         Returns the smallest of three integers.
 *
 * Inputs:
 *         a : first value.
 *         b : second value.
 *         c : third value.
 *
 * Returns:
 *         The smallest of a, b, and c.
 *****************************************************************************/
int min3(int a, int b, int c)
{
    int m = (a < b) ? a : b;

    return (m < c) ? m : c;
}

/*****************************************************************************
 * Name: editDistance
 *
 * Description:
 *         Classic Levenshtein distance via a 2D DP table: table[i][j] is the
 *         minimum number of single-character insertions, deletions, and
 *         substitutions needed to transform the first i characters of src
 *         into the first j characters of dst. If the current characters
 *         match, no edit is needed and the diagonal carries over unchanged;
 *         otherwise the cheapest of insert/delete/substitute is taken.
 *
 * Inputs:
 *         src : the source string.
 *         dst : the destination string.
 *
 * Returns:
 *         The minimum edit distance between src and dst.
 *****************************************************************************/
int editDistance(const char *src, const char *dst)
{
    int table[MAX_LEN + 1][MAX_LEN + 1];
    int srcLen;
    int dstLen;
    int i;
    int j;

    srcLen = strlen(src);
    dstLen = strlen(dst);

    for (i = 0; i <= srcLen; i++)
    {
        for (j = 0; j <= dstLen; j++)
        {
            if (i == 0)
            {
                table[i][j] = j;
            }
            else if (j == 0)
            {
                table[i][j] = i;
            }
            else if (src[i - 1] == dst[j - 1])
            {
                table[i][j] = table[i - 1][j - 1];
            }
            else
            {
                int deleteOp = table[i - 1][j];
                int insertOp = table[i][j - 1];
                int substituteOp = table[i - 1][j - 1];

                table[i][j] = 1 + min3(deleteOp, insertOp, substituteOp);
            }
        }
    }

    return table[srcLen][dstLen];
}

int main()
{
    const char *src = "kitten";
    const char *dst = "sitting";

    // Same DP shape used by diff tools (minimal edit script between two
    // files), spell-checkers (nearest dictionary word), and DNA sequence
    // alignment (minimal mutations between two sequences).
    printf("edit distance(\"%s\", \"%s\") = %d\n", src, dst, editDistance(src, dst));

    return 0;
}
