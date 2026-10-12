// Greedy on digit strings: remove k digits for the smallest number (monotonic
// stack) and arrange numbers to form the largest value.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void remove_k(const char *num, int k, char *out)
{
    int top = 0;
    for (const char *p = num; *p; p++)
    {
        while (k > 0 && top > 0 && out[top - 1] > *p)   /* a larger digit before a smaller one: dropping it helps */
        {
            top--;
            k--;
        }
        out[top++] = *p;
    }
    top -= k;                                           /* digits still to remove come off the end */
    int start = 0;
    while (start < top - 1 && out[start] == '0') start++;
    memmove(out, out + start, top - start);
    out[top - start] = '\0';
    if (!out[0]) strcpy(out, "0");
}

static int bigger_first(const void *a, const void *b)
{
    char x[24], y[24];
    snprintf(x, sizeof x, "%d%d", *(const int *)a, *(const int *)b);   /* compare the two concatenations */
    snprintf(y, sizeof y, "%d%d", *(const int *)b, *(const int *)a);
    return strcmp(y, x);
}

int main(void)
{
    char out[32];
    remove_k("1432219", 3, out);
    printf("remove 3 digits from 1432219 -> %s\n", out);
    remove_k("10200", 1, out);
    printf("remove 1 digit  from 10200   -> %s\n", out);
    remove_k("10", 2, out);
    printf("remove 2 digits from 10      -> %s\n", out);

    int a[] = {3, 30, 34, 5, 9};
    qsort(a, 5, sizeof a[0], bigger_first);
    printf("largest number from 3 30 34 5 9 -> ");
    for (int i = 0; i < 5; i++) printf("%d", a[i]);
    printf("\n");
    return 0;
}
