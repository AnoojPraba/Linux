// Arbitrary-precision arithmetic on decimal strings: addition, multiplication
// and factorial of large numbers.

#include <stdio.h>
#include <string.h>

static void add(const char *a, const char *b, char *out)
{
    int i = (int)strlen(a) - 1, j = (int)strlen(b) - 1, carry = 0, k = 0;
    char rev[128];
    while (i >= 0 || j >= 0 || carry)
    {
        int s = carry + (i >= 0 ? a[i--] - '0' : 0) + (j >= 0 ? b[j--] - '0' : 0);
        rev[k++] = (char)('0' + s % 10);
        carry = s / 10;
    }
    for (int x = 0; x < k; x++) out[x] = rev[k - 1 - x];
    out[k] = '\0';
}

static void multiply(const char *a, const char *b, char *out)
{
    int n = (int)strlen(a), m = (int)strlen(b), res[128] = {0};
    for (int i = n - 1; i >= 0; i--)                     /* schoolbook: digit i of a times digit j of b lands at i+j+1 */
        for (int j = m - 1; j >= 0; j--)
        {
            int p = (a[i] - '0') * (b[j] - '0') + res[i + j + 1];
            res[i + j + 1] = p % 10;
            res[i + j] += p / 10;
        }
    int k = 0, start = 0;
    while (start < n + m - 1 && res[start] == 0) start++;
    for (int i = start; i < n + m; i++) out[k++] = (char)('0' + res[i]);
    out[k] = '\0';
}

static void factorial(int n, char *out)
{
    int digits[200] = {1}, len = 1;                      /* little-endian digit array */
    for (int x = 2; x <= n; x++)
    {
        int carry = 0;
        for (int i = 0; i < len; i++)
        {
            int p = digits[i] * x + carry;
            digits[i] = p % 10;
            carry = p / 10;
        }
        while (carry) { digits[len++] = carry % 10; carry /= 10; }
    }
    for (int i = 0; i < len; i++) out[i] = (char)('0' + digits[len - 1 - i]);
    out[len] = '\0';
}

int main(void)
{
    char out[256];
    add("99999999999999999999", "1", out);
    printf("99999999999999999999 + 1 = %s\n", out);
    multiply("123456789", "987654321", out);
    printf("123456789 * 987654321 = %s\n", out);
    multiply("99999999999999999999", "99999999999999999999", out);
    printf("(10^20 - 1)^2 = %s\n", out);
    factorial(30, out);
    printf("30! = %s\n", out);
    return 0;
}
