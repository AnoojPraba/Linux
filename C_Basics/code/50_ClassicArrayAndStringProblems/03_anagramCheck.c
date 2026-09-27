#include <stdio.h>
#include <string.h>

#define ALPHABET_SIZE 26

/* O(n log n) alternative (not implemented here): sort both strings and
 * compare the results character-by-character - two strings are anagrams
 * iff their sorted forms are identical. The frequency-count approach below
 * is the classic O(n) improvement for lowercase-only input, since it never
 * needs to sort. */

/*****************************************************************************
 * Name: isAnagram
 *
 * Description:
 *         Determines whether two lowercase strings are anagrams of each
 *         other, using an array of 26 character-frequency counts: increment
 *         the count for each character of the first string, decrement for
 *         each character of the second, and the strings are anagrams iff
 *         every count ends at zero (and the lengths matched to begin with).
 *
 * Inputs:
 *         a : first lowercase, null-terminated string.
 *         b : second lowercase, null-terminated string.
 *
 * Returns:
 *         1 if a and b are anagrams, 0 otherwise.
 *****************************************************************************/
int isAnagram(const char *a, const char *b)
{
    int counts[ALPHABET_SIZE] = {0};
    int i;

    if (strlen(a) != strlen(b))
    {
        return 0;
    }

    for (i = 0; a[i] != '\0'; i++)
    {
        counts[a[i] - 'a']++;
        counts[b[i] - 'a']--;
    }

    for (i = 0; i < ALPHABET_SIZE; i++)
    {
        if (counts[i] != 0)
        {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    printf("isAnagram(\"listen\", \"silent\") = %d\n", isAnagram("listen", "silent"));
    printf("isAnagram(\"hello\", \"world\") = %d\n", isAnagram("hello", "world"));

    return 0;
}
