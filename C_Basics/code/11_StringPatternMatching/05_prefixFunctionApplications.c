// Applications of the KMP prefix function: borders, smallest period, counting
// occurrences and the shortest palindrome prefix extension.

#include <stdio.h>
#include <string.h>

/* pi[i] = length of the longest proper prefix of s[0..i] that is also its suffix */
static void prefix_function(const char *s, int n, int *pi)
{
    pi[0] = 0;
    for (int i = 1, k = 0; i < n; i++)
    {
        while (k > 0 && s[i] != s[k]) k = pi[k - 1];
        if (s[i] == s[k]) k++;
        pi[i] = k;
    }
}

int main(void)
{
    const char *s = "abcabcabc";
    int n = (int)strlen(s), pi[64];
    prefix_function(s, n, pi);
    printf("prefix function of %s:", s);
    for (int i = 0; i < n; i++) printf(" %d", pi[i]);

    int period = n - pi[n - 1];                          /* smallest p such that s repeats with period p */
    printf("\nsmallest period = %d, is a repetition of a shorter block: %s\n", period, (n % period == 0 && period < n) ? "yes" : "no");

    printf("borders (prefix = suffix):");
    for (int k = pi[n - 1]; k > 0; k = pi[k - 1]) printf(" \"%.*s\"", k, s);
    printf("\n");

    /* shortest palindrome by adding characters in FRONT: use s + '#' + reverse(s) */
    const char *t = "aacecaaa";
    int m = (int)strlen(t);
    char both[64];
    snprintf(both, sizeof both, "%s#", t);
    for (int i = 0; i < m; i++) both[m + 1 + i] = t[m - 1 - i];
    both[2 * m + 1] = '\0';
    int pi2[64];
    prefix_function(both, 2 * m + 1, pi2);
    int keep = pi2[2 * m];                               /* length of the longest palindromic prefix */
    printf("longest palindromic prefix of %s has length %d; shortest palindrome by prepending: ", t, keep);
    for (int i = m - 1; i >= keep; i--) putchar(t[i]);
    printf("%s\n", t);
    return 0;
}
