// Bijection checks between strings: isomorphic strings, word pattern matching
// and valid anagram groups by signature.

#include <stdio.h>
#include <string.h>

static int isomorphic(const char *s, const char *t)
{
    int map_st[128], map_ts[128];
    memset(map_st, -1, sizeof map_st);
    memset(map_ts, -1, sizeof map_ts);
    if (strlen(s) != strlen(t)) return 0;
    for (int i = 0; s[i]; i++)
    {
        if (map_st[(int)s[i]] < 0 && map_ts[(int)t[i]] < 0)
        {
            map_st[(int)s[i]] = t[i];                    /* a one-to-one pairing needs both directions checked */
            map_ts[(int)t[i]] = s[i];
        }
        else if (map_st[(int)s[i]] != t[i] || map_ts[(int)t[i]] != s[i]) return 0;
    }
    return 1;
}

static int word_pattern(const char *pattern, const char *text)
{
    char words[16][16], copy[128];
    int n = 0;
    snprintf(copy, sizeof copy, "%s", text);
    for (char *w = strtok(copy, " "); w; w = strtok(NULL, " ")) snprintf(words[n++], 16, "%s", w);
    if ((int)strlen(pattern) != n) return 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if ((pattern[i] == pattern[j]) != (strcmp(words[i], words[j]) == 0)) return 0;
    return 1;
}

int main(void)
{
    printf("egg / add  : %d\n", isomorphic("egg", "add"));
    printf("foo / bar  : %d\n", isomorphic("foo", "bar"));
    printf("badc / baba: %d\n", isomorphic("badc", "baba"));
    printf("pattern abba vs \"dog cat cat dog\": %d\n", word_pattern("abba", "dog cat cat dog"));
    printf("pattern abba vs \"dog cat cat fish\": %d\n", word_pattern("abba", "dog cat cat fish"));
    printf("pattern aaaa vs \"dog cat cat dog\": %d\n", word_pattern("aaaa", "dog cat cat dog"));
    return 0;
}
