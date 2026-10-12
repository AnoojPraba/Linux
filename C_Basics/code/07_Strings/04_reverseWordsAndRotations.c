// In-place string manipulations: reverse the words of a sentence, rotate a
// string with the three-reversal trick and test rotation with doubling.

#include <stdio.h>
#include <string.h>

static void reverse(char *s, int lo, int hi)
{
    for (; lo < hi; lo++, hi--) { char t = s[lo]; s[lo] = s[hi]; s[hi] = t; }
}

static void reverse_words(char *s)
{
    int n = (int)strlen(s);
    reverse(s, 0, n - 1);                               /* reverse everything, then every word back */
    for (int i = 0, start = 0; i <= n; i++)
        if (s[i] == ' ' || s[i] == '\0')
        {
            reverse(s, start, i - 1);
            start = i + 1;
        }
}

static void rotate_left(char *s, int k)
{
    int n = (int)strlen(s);
    k %= n;
    reverse(s, 0, k - 1);                               /* three reversals rotate in O(n) time and O(1) space */
    reverse(s, k, n - 1);
    reverse(s, 0, n - 1);
}

static int is_rotation(const char *a, const char *b)
{
    char doubled[64];
    if (strlen(a) != strlen(b)) return 0;
    snprintf(doubled, sizeof doubled, "%s%s", a, a);
    return strstr(doubled, b) != NULL;                  /* every rotation of a is a substring of a+a */
}

int main(void)
{
    char s[] = "the sky is blue";
    reverse_words(s);
    printf("reversed words: %s\n", s);

    char r[] = "abcdefg";
    rotate_left(r, 2);
    printf("rotate left by 2: %s\n", r);

    printf("waterbottle is a rotation of erbottlewat: %d\n", is_rotation("waterbottle", "erbottlewat"));
    printf("abcd is a rotation of acbd: %d\n", is_rotation("abcd", "acbd"));
    return 0;
}
