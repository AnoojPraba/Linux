#include <stdio.h>

#define ALPHABET_SIZE 26

/*****************************************************************************
 * Name: firstNonRepeatingChar
 *
 * Description:
 *         Finds the first non-repeating character in a lowercase string,
 *         using a frequency count in a first pass (counting how many times
 *         each character appears), then a second pass over the string in
 *         order to find the first character whose count is exactly 1.
 *
 * Inputs:
 *         str : lowercase, null-terminated string.
 *
 * Returns:
 *         The first non-repeating character, or '\0' if none exists.
 *****************************************************************************/
char firstNonRepeatingChar(const char *str)
{
    int counts[ALPHABET_SIZE] = {0};
    int i;

    for (i = 0; str[i] != '\0'; i++)
    {
        counts[str[i] - 'a']++;
    }

    for (i = 0; str[i] != '\0'; i++)
    {
        if (counts[str[i] - 'a'] == 1)
        {
            return str[i];
        }
    }

    return '\0';
}

int main(void)
{
    const char *str = "swiss";
    char result = firstNonRepeatingChar(str);

    if (result != '\0')
    {
        printf("first non-repeating char in \"%s\" = '%c'\n", str, result);
    }
    else
    {
        printf("no non-repeating char in \"%s\"\n", str);
    }

    return 0;
}
