// Articulation points and bridges with Tarjan's discovery-time/low-link DFS.

#include <stdio.h>

#define N 9
static int adj[N][N], deg[N];
static int disc[N], low[N], timer_, is_art[N];

static void edge(int u, int v) { adj[u][deg[u]++] = v; adj[v][deg[v]++] = u; }

static void dfs(int u, int parent)
{
    disc[u] = low[u] = ++timer_;
    int children = 0;
    for (int i = 0; i < deg[u]; i++)
    {
        int v = adj[u][i];
        if (v == parent)
            continue;
        if (disc[v])                                   /* back edge */
        {
            if (disc[v] < low[u]) low[u] = disc[v];
        }
        else
        {
            children++;
            dfs(v, u);
            if (low[v] < low[u]) low[u] = low[v];
            if (low[v] > disc[u])                      /* nothing below v reaches above u: u-v is a bridge */
                printf("bridge: %d - %d\n", u, v);
            if (parent >= 0 && low[v] >= disc[u])      /* v's subtree cannot bypass u */
                is_art[u] = 1;
        }
    }
    if (parent < 0 && children > 1)                    /* the root is a cut vertex only with 2+ DFS children */
        is_art[u] = 1;
}

int main(void)
{
    edge(0, 1); edge(1, 2); edge(2, 0);                /* triangle 0-1-2 */
    edge(1, 3); edge(3, 4); edge(4, 5); edge(5, 3);    /* triangle 3-4-5 joined by the bridge 1-3 */
    edge(5, 6);                                        /* a tail */
    edge(7, 8);                                        /* a separate component */

    for (int i = 0; i < N; i++)
        if (!disc[i]) dfs(i, -1);

    printf("articulation points:");
    for (int i = 0; i < N; i++)
        if (is_art[i]) printf(" %d", i);
    printf("\n");
    return 0;
}
