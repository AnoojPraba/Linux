// Maximum XOR of two numbers with a binary trie: walk the opposite bit at
// every level.

#include <stdio.h>
#include <string.h>

#define BITS 8
#define MAXN 512

static int child[MAXN][2], nodes = 1;

static void insert(unsigned x)
{
    int v = 0;
    for (int b = BITS - 1; b >= 0; b--)
    {
        int bit = x >> b & 1;
        if (!child[v][bit]) child[v][bit] = nodes++;
        v = child[v][bit];
    }
}

static unsigned best_partner(unsigned x)
{
    int v = 0;
    unsigned result = 0;
    for (int b = BITS - 1; b >= 0; b--)
    {
        int want = !(x >> b & 1);                       /* the opposite bit makes this bit of the xor a 1 */
        if (child[v][want]) { result |= 1u << b; v = child[v][want]; }
        else v = child[v][!want];
    }
    return result;                                      /* x xor (best stored number) */
}

int main(void)
{
    unsigned a[] = {3, 10, 5, 25, 2, 8};
    memset(child, 0, sizeof child);
    unsigned best = 0;
    for (int i = 0; i < 6; i++)
    {
        insert(a[i]);
        unsigned r = best_partner(a[i]);
        if (r > best) best = r;
    }
    printf("maximum xor of two numbers in {3,10,5,25,2,8} = %u (5 ^ 25)\n", best);

    unsigned brute = 0;
    for (int i = 0; i < 6; i++)
        for (int j = i + 1; j < 6; j++)
            if ((a[i] ^ a[j]) > brute) brute = a[i] ^ a[j];
    printf("brute force agrees: %s\n", brute == best ? "yes" : "no");

    unsigned pre[7] = {0};                              /* maximum xor subarray: xor of a subarray = prefix[j] ^ prefix[i] */
    for (int i = 0; i < 6; i++) pre[i + 1] = pre[i] ^ a[i];
    memset(child, 0, sizeof child);
    nodes = 1;
    unsigned sub = 0;
    insert(0);
    for (int i = 1; i <= 6; i++)
    {
        unsigned r = best_partner(pre[i]);
        if (r > sub) sub = r;
        insert(pre[i]);
    }
    printf("maximum xor of any contiguous subarray = %u\n", sub);
    return 0;
}
