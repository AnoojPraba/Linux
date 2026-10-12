// Validating formatted strings: numbers (sign, decimals, exponent) with a
// state machine, IPv4 addresses and a palindrome ignoring punctuation.

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int valid_number(const char *s)
{
    int i = 0, digits = 0, dot = 0;
    if (s[i] == '+' || s[i] == '-') i++;
    for (; s[i] && s[i] != 'e' && s[i] != 'E'; i++)
    {
        if (isdigit((unsigned char)s[i])) digits++;
        else if (s[i] == '.' && !dot) dot = 1;
        else return 0;
    }
    if (!digits) return 0;                              /* at least one digit before any exponent */
    if (s[i])                                           /* exponent part: optional sign then digits */
    {
        i++;
        if (s[i] == '+' || s[i] == '-') i++;
        if (!isdigit((unsigned char)s[i])) return 0;
        for (; s[i]; i++)
            if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

static int valid_ipv4(const char *s)
{
    char copy[32];
    int parts = 0;
    snprintf(copy, sizeof copy, "%s", s);
    for (char *tok = strtok(copy, "."); tok; tok = strtok(NULL, "."), parts++)
    {
        int len = (int)strlen(tok);
        if (len == 0 || len > 3 || (len > 1 && tok[0] == '0')) return 0;   /* no leading zeros */
        for (int i = 0; i < len; i++)
            if (!isdigit((unsigned char)tok[i])) return 0;
        if (atoi(tok) > 255) return 0;
    }
    int dots = 0;
    for (const char *c = s; *c; c++) dots += *c == '.';
    return parts == 4 && dots == 3;
}

static int palindrome_alnum(const char *s)
{
    int l = 0, r = (int)strlen(s) - 1;
    while (l < r)
    {
        while (l < r && !isalnum((unsigned char)s[l])) l++;
        while (l < r && !isalnum((unsigned char)s[r])) r--;
        if (tolower((unsigned char)s[l++]) != tolower((unsigned char)s[r--])) return 0;
    }
    return 1;
}

int main(void)
{
    const char *nums[] = {"2", "-0.1", "3.14e10", "1e", ".", "--6", "+.8", "e9"};
    for (int i = 0; i < 8; i++) printf("valid number %-8s: %d\n", nums[i], valid_number(nums[i]));
    const char *ips[] = {"192.168.1.1", "256.1.1.1", "1.2.3", "01.2.3.4", "1.2.3.4.5"};
    for (int i = 0; i < 5; i++) printf("valid IPv4 %-12s: %d\n", ips[i], valid_ipv4(ips[i]));
    printf("\"A man, a plan, a canal: Panama\" is a palindrome: %d\n", palindrome_alnum("A man, a plan, a canal: Panama"));
    return 0;
}
