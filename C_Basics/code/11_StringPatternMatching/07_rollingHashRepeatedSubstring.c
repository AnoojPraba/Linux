// Polynomial rolling hash plus binary search on length: longest repeated
// substring and a duplicate-window finder.

#include <stdio.h>
#include <string.h>

#define MOD 1000000007ULL
#define BASE 131ULL

static const char *S;
static int n;

/* does some substring of the given length occur twice? returns its start of the second occurrence or -1 */
static int repeated(int len)
{
    static unsigned long long seen_hash[64];
    static int seen_pos[64];
    int count = 0;
    unsigned long long h = 0, pw = 1;
    for (int i = 0; i < len; i++) pw = pw * BASE % MOD;
    for (int i = 0; i < n; i++)
    {
        h = (h * BASE + (unsigned char)S[i]) % MOD;
        if (i >= len) h = (h + MOD - (unsigned char)S[i - len] * pw % MOD) % MOD;   /* drop the character leaving the window */
        if (i >= len - 1)
        {
            int start = i - len + 1;
            for (int k = 0; k < count; k++)
                if (seen_hash[k] == h && strncmp(S + seen_pos[k], S + start, len) == 0)   /* verify to rule out a collision */
                    return start;
            seen_hash[count] = h;
            seen_pos[count++] = start;
        }
    }
    return -1;
}

int main(void)
{
    S = "abcxabcdabcxabc";
    n = (int)strlen(S);
    int lo = 1, hi = n - 1, best = 0, at = -1;
    while (lo <= hi)                                      /* "a repeat of length L exists" is monotone in L */
    {
        int mid = (lo + hi) / 2, p = repeated(mid);
        if (p >= 0) { best = mid; at = p; lo = mid + 1; }
        else hi = mid - 1;
    }
    printf("longest repeated substring of %s: %.*s (length %d)\n", S, best, S + at, best);
    return 0;
}
