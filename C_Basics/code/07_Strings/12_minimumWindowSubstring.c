// Sliding window with character counts: minimum window containing all
// characters of a target, and the longest substring with at most k distinct
// characters.

#include <stdio.h>
#include <string.h>

static void min_window(const char *s, const char *t, int *best_start, int *best_len)
{
    int need[128] = {0}, missing = (int)strlen(t), n = (int)strlen(s);
    for (const char *c = t; *c; c++) need[(int)*c]++;
    *best_len = n + 1;
    for (int l = 0, r = 0; r < n; r++)
    {
        if (need[(int)s[r]]-- > 0) missing--;            /* this character was still needed */
        while (missing == 0)                             /* the window is valid: shrink from the left while it stays valid */
        {
            if (r - l + 1 < *best_len) { *best_len = r - l + 1; *best_start = l; }
            if (++need[(int)s[l++]] > 0) missing++;
        }
    }
}

static int longest_k_distinct(const char *s, int k)
{
    int cnt[128] = {0}, distinct = 0, best = 0, n = (int)strlen(s);
    for (int l = 0, r = 0; r < n; r++)
    {
        if (cnt[(int)s[r]]++ == 0) distinct++;
        while (distinct > k)
            if (--cnt[(int)s[l++]] == 0) distinct--;
        if (r - l + 1 > best) best = r - l + 1;
    }
    return best;
}

int main(void)
{
    int start = 0, len;
    min_window("ADOBECODEBANC", "ABC", &start, &len);
    printf("minimum window of ADOBECODEBANC covering ABC: %.*s\n", len, "ADOBECODEBANC" + start);
    min_window("a", "aa", &start, &len);
    printf("window for a covering aa exists: %s\n", len <= 1 ? "yes" : "no");
    printf("longest substring with at most 2 distinct characters in eceba: %d\n", longest_k_distinct("eceba", 2));
    printf("longest substring with at most 3 distinct characters in aabacbebebe: %d\n", longest_k_distinct("aabacbebebe", 3));
    return 0;
}
