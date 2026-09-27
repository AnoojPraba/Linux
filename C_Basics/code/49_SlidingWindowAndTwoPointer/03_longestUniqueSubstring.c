#include <stdio.h>
#include <string.h>

#define ASCII_RANGE 256

/*****************************************************************************
 * Name: longestUniqueSubstringLength
 *
 * Description:
 *         Variable-size sliding window: length of the longest substring
 *         without repeating characters. Expand the window by advancing a
 *         right pointer; whenever the incoming character is already inside
 *         the current window, shrink from the left until the duplicate is
 *         gone. lastSeenAt tracks the most recent index of each character so
 *         the left pointer can jump directly past the duplicate instead of
 *         stepping one at a time - still O(n) overall.
 *
 * Inputs:
 *         str : the input string.
 *
 * Returns:
 *         The length of the longest substring without repeating characters.
 *****************************************************************************/
int longestUniqueSubstringLength(const char *str)
{
    int lastSeenAt[ASCII_RANGE];
    int left;
    int right;
    int maxLen;
    int len;

    for (left = 0; left < ASCII_RANGE; left++)
    {
        lastSeenAt[left] = -1;
    }

    left = 0;
    maxLen = 0;
    len = (int)strlen(str);

    for (right = 0; right < len; right++)
    {
        unsigned char c = (unsigned char)str[right];

        if (lastSeenAt[c] >= left)
        {
            left = lastSeenAt[c] + 1;
        }

        lastSeenAt[c] = right;

        if (right - left + 1 > maxLen)
        {
            maxLen = right - left + 1;
        }
    }

    return maxLen;
}

int main()
{
    const char *str = "abcabcbb";

    printf("longest unique substring length of \"%s\" = %d\n", str,
           longestUniqueSubstringLength(str));

    return 0;
}
