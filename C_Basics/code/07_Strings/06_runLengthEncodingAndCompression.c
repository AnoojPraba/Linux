// Run-length encoding and decoding, plus in-place character compression like
// a3b2.

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void encode(const char *s, char *out)
{
    while (*s)
    {
        const char *p = s;
        while (*p == *s) p++;                           /* find the end of the run */
        out += sprintf(out, "%c%d", *s, (int)(p - s));
        s = p;
    }
    *out = '\0';
}

static void decode(const char *s, char *out)
{
    while (*s)
    {
        char c = *s++;
        int count = strtol(s, (char **)&s, 10);          /* the digits after the character */
        while (count--) *out++ = c;
    }
    *out = '\0';
}

/* in place: "aabcccccaaa" -> "a2bc5a3" (a count of 1 is omitted); returns the new length */
static int compress_in_place(char *s)
{
    int w = 0, n = (int)strlen(s);
    for (int i = 0; i < n;)
    {
        int j = i;
        while (j < n && s[j] == s[i]) j++;
        s[w++] = s[i];
        if (j - i > 1)
        {
            char digits[12];
            int len = snprintf(digits, sizeof digits, "%d", j - i);
            memcpy(s + w, digits, len);
            w += len;
        }
        i = j;
    }
    s[w] = '\0';
    return w;
}

int main(void)
{
    char enc[64], dec[64];
    encode("wwwwaaadexxxxxx", enc);
    decode(enc, dec);
    printf("encoded: %s\ndecoded: %s\n", enc, dec);

    char t[] = "aabcccccaaa";
    int len = compress_in_place(t);
    printf("in-place compression: %s (length %d)\n", t, len);
    return 0;
}
