#include <stdio.h>
#include <string.h>

#define HASH_BASE 256
#define HASH_MOD 101

// Rabin-Karp: compare a rolling hash of each text window against the
// pattern's hash first, and only fall back to a full character comparison
// when the hashes match (guards against hash collisions). The rolling hash
// lets each window's hash be derived from the previous one in O(1), instead
// of recomputing it from scratch.
void rabinKarpSearch(const char *text, const char *pattern)
{
    int n = (int) strlen(text);
    int m = (int) strlen(pattern);
    int patternHash = 0;
    int windowHash = 0;
    int highOrder = 1;
    int i;

    if (m > n)
    {
        return;
    }

    // highOrder = HASH_BASE^(m-1) % HASH_MOD, needed to remove the
    // leading digit's contribution when the window slides forward.
    for (i = 0; i < m - 1; i++)
    {
        highOrder = (highOrder * HASH_BASE) % HASH_MOD;
    }

    for (i = 0; i < m; i++)
    {
        patternHash = (HASH_BASE * patternHash + pattern[i]) % HASH_MOD;
        windowHash = (HASH_BASE * windowHash + text[i]) % HASH_MOD;
    }

    for (i = 0; i <= n - m; i++)
    {
        if (patternHash == windowHash)
        {
            if (strncmp(text + i, pattern, m) == 0)
            {
                printf("Match at index %d\n", i);
            }
        }

        // Slide the window: drop the leading character's contribution,
        // shift the remaining digits up, and bring in the next character -
        // all in O(1), instead of rehashing the whole window.
        if (i < n - m)
        {
            windowHash = (HASH_BASE * (windowHash - text[i] * highOrder) + text[i + m])
                % HASH_MOD;
            if (windowHash < 0)
            {
                windowHash += HASH_MOD;
            }
        }
    }
}

int main()
{
    const char *text = "abxabcabcaby";
    const char *pattern = "abcaby";

    rabinKarpSearch(text, pattern);

    return 0;
}
