// Tree algorithms: parent/depth by DFS, lowest common ancestor with binary
// lifting, distance between nodes and tree diameter.

#include <stdio.h>

#define N 10
#define LOG 4
static int adj[N][N], deg[N];
static int up[LOG][N], depth[N];

static void edge(int a, int b) { adj[a][deg[a]++] = b; adj[b][deg[b]++] = a; }

static void dfs(int u, int p)
{
    up[0][u] = p;
    for (int k = 1; k < LOG; k++)
        up[k][u] = up[k - 1][up[k - 1][u]];            /* the 2^k-th ancestor = the 2^(k-1)-th of the 2^(k-1)-th */
    for (int i = 0; i < deg[u]; i++)
        if (adj[u][i] != p)
        {
            depth[adj[u][i]] = depth[u] + 1;
            dfs(adj[u][i], u);
        }
}

static int lca(int a, int b)
{
    if (depth[a] < depth[b]) { int t = a; a = b; b = t; }
    int diff = depth[a] - depth[b];
    for (int k = 0; k < LOG; k++)
        if (diff >> k & 1) a = up[k][a];               /* lift the deeper node to the same depth */
    if (a == b) return a;
    for (int k = LOG - 1; k >= 0; k--)
        if (up[k][a] != up[k][b])                      /* jump both while the ancestors differ */
        {
            a = up[k][a];
            b = up[k][b];
        }
    return up[0][a];
}

static int far_node, far_dist;
static void farthest(int u, int p, int d)
{
    if (d > far_dist) { far_dist = d; far_node = u; }
    for (int i = 0; i < deg[u]; i++)
        if (adj[u][i] != p) farthest(adj[u][i], u, d + 1);
}

int main(void)
{
    edge(0, 1); edge(0, 2); edge(1, 3); edge(1, 4); edge(2, 5); edge(4, 6); edge(4, 7); edge(5, 8); edge(8, 9);
    dfs(0, 0);                                         /* the root is its own parent */

    int pairs[][2] = {{3, 6}, {6, 7}, {3, 9}, {7, 7}, {5, 9}};
    for (int i = 0; i < 5; i++)
    {
        int a = pairs[i][0], b = pairs[i][1], l = lca(a, b);
        printf("lca(%d, %d) = %d, distance = %d\n", a, b, l, depth[a] + depth[b] - 2 * depth[l]);
    }

    far_dist = -1; farthest(0, -1, 0);                 /* the diameter by two sweeps: farthest from any node, then from that node */
    int a = far_node;
    far_dist = -1; farthest(a, -1, 0);
    printf("tree diameter = %d edges (from %d to %d)\n", far_dist, a, far_node);
    return 0;
}
