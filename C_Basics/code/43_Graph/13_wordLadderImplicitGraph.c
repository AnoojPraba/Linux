// BFS on an implicit graph: word ladder where two words are adjacent if they
// differ in one letter.

#include <stdio.h>
#include <string.h>

#define MAXW 12
static const char *words[MAXW] = {"hit", "hot", "dot", "dog", "cog", "lot", "log", "hog", "cot", "cat", "hat", "bat"};
#define W 12

static int one_apart(const char *a, const char *b)
{
    int diff = 0;
    for (int i = 0; a[i]; i++)
        if (a[i] != b[i] && ++diff > 1) return 0;
    return diff == 1;
}

int main(void)
{
    const char *src = "hit", *dst = "cog";
    int s = -1, d = -1;
    for (int i = 0; i < W; i++)
    {
        if (!strcmp(words[i], src)) s = i;
        if (!strcmp(words[i], dst)) d = i;
    }
    int dist[W], parent[W], q[W], h = 0, t = 0;
    for (int i = 0; i < W; i++) dist[i] = -1;
    dist[s] = 0; parent[s] = -1; q[t++] = s;
    while (h < t)
    {
        int u = q[h++];
        for (int v = 0; v < W; v++)                    /* neighbours are generated on the fly, no stored edges */
            if (dist[v] < 0 && one_apart(words[u], words[v]))
            {
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q[t++] = v;
            }
    }
    if (dist[d] < 0) { printf("no ladder\n"); return 0; }
    int path[W], n = 0;
    for (int v = d; v >= 0; v = parent[v]) path[n++] = v;
    printf("shortest ladder (%d words):", dist[d] + 1);
    for (int i = n - 1; i >= 0; i--) printf(" %s", words[path[i]]);
    printf("\n");
    return 0;
}
