// Aho-Corasick: find all occurrences of many patterns in one pass using a trie
// with failure links.

#include <stdio.h>
#include <string.h>

#define MAXN 64
#define ALPHA 26
static int next_[MAXN][ALPHA], fail[MAXN], out[MAXN], nn = 1;      /* out[v] = id+1 of a pattern ending at v, or 0 */
static int dict_link[MAXN];                                         /* nearest suffix node that ends a pattern */
static const char *pats[] = {"he", "she", "his", "hers"};

static void insert(const char *p, int id)
{
    int v = 0;
    for (; *p; p++)
    {
        int c = *p - 'a';
        if (!next_[v][c]) next_[v][c] = nn++;
        v = next_[v][c];
    }
    out[v] = id + 1;
}

static void build(void)
{
    int q[MAXN], h = 0, t = 0;
    for (int c = 0; c < ALPHA; c++)
        if (next_[0][c]) q[t++] = next_[0][c];
    while (h < t)
    {
        int v = q[h++];
        for (int c = 0; c < ALPHA; c++)
        {
            int u = next_[v][c];
            if (!u) { next_[v][c] = next_[fail[v]][c]; continue; }   /* missing edge: follow the failure link's transition */
            fail[u] = next_[fail[v]][c];
            dict_link[u] = out[fail[u]] ? fail[u] : dict_link[fail[u]];
            q[t++] = u;
        }
    }
}

int main(void)
{
    for (int i = 0; i < 4; i++) insert(pats[i], i);
    build();

    const char *text = "ushers";
    int v = 0;
    for (int i = 0; text[i]; i++)
    {
        v = next_[v][text[i] - 'a'];
        for (int u = out[v] ? v : dict_link[v]; u; u = dict_link[u])   /* report every pattern ending here */
            printf("found \"%s\" ending at index %d\n", pats[out[u] - 1], i);
    }
    return 0;
}
