#include <stdio.h>
#include <string.h>

#define MAX_WORDS 6
#define MAX_LEN 32

void sortString(char *str)
{
    int len = (int)strlen(str);
    int i;
    int j;
    char temp;

    for (i = 0; i < (len - 1); i++)
    {
        for (j = 0; j < (len - 1 - i); j++)
        {
            if (str[j] > str[j + 1])
            {
                temp = str[j];
                str[j] = str[j + 1];
                str[j + 1] = temp;
            }
        }
    }
}

/*****************************************************************************
 * Name: groupAnagrams
 *
 * Description:
 *         Groups an array of strings so that anagrams end up together, by
 *         computing a sorted-character signature for each word (its sorted
 *         letters double as a hash key - two words are anagrams iff their
 *         sorted forms are identical) and printing every word alongside the
 *         other not-yet-printed words sharing that same signature.
 *
 * Inputs:
 *         words : array of null-terminated strings.
 *         n     : number of strings in words.
 *
 * Returns:
 *         None
 *****************************************************************************/
void groupAnagrams(char words[][MAX_LEN], int n)
{
    char signatures[MAX_WORDS][MAX_LEN];
    int used[MAX_WORDS] = {0};
    int i;
    int j;

    for (i = 0; i < n; i++)
    {
        strcpy(signatures[i], words[i]);
        sortString(signatures[i]);
    }

    for (i = 0; i < n; i++)
    {
        if (used[i])
        {
            continue;
        }
        printf("group: %s", words[i]);
        used[i] = 1;
        for (j = i + 1; j < n; j++)
        {
            if ((!used[j]) && (strcmp(signatures[i], signatures[j]) == 0))
            {
                printf(", %s", words[j]);
                used[j] = 1;
            }
        }
        printf("\n");
    }
}

int main(void)
{
    char words[MAX_WORDS][MAX_LEN] = {"eat", "tea", "tan", "ate", "nat", "bat"};

    groupAnagrams(words, MAX_WORDS);

    return 0;
}
