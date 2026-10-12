// Dinic's maximum flow: BFS level graph plus blocking-flow DFS with current-
// arc pointers, O(V^2 E).

#include <stdio.h>
#include <string.h>

#define MAXV 8
#define MAXE 64
typedef struct { int to, cap; } Edge;
static Edge edges[MAXE];
static int ne;
static int adj[MAXV][MAXE], deg[MAXV];
static int level[MAXV], it[MAXV];

static void add_edge(int a, int b, int c)
{
    edges[ne] = (Edge){b, c}; adj[a][deg[a]++] = ne++;
    edges[ne] = (Edge){a, 0}; adj[b][deg[b]++] = ne++;     /* the paired reverse edge is always index ^ 1 */
}

static int bfs(int s, int t, int n)
{
    int q[MAXV], h = 0, tl = 0;
    for (int i = 0; i < n; i++) level[i] = -1;
    level[s] = 0; q[tl++] = s;
    while (h < tl)
    {
        int u = q[h++];
        for (int i = 0; i < deg[u]; i++)
        {
            Edge e = edges[adj[u][i]];
            if (e.cap > 0 && level[e.to] < 0)
            {
                level[e.to] = level[u] + 1;
                q[tl++] = e.to;
            }
        }
    }
    return level[t] >= 0;
}

static int dfs(int u, int t, int pushed)
{
    if (u == t || pushed == 0) return pushed;
    for (; it[u] < deg[u]; it[u]++)                    /* current-arc pointer: never rescan a dead edge */
    {
        int id = adj[u][it[u]];
        Edge *e = &edges[id];
        if (e->cap > 0 && level[e->to] == level[u] + 1)
        {
            int got = dfs(e->to, t, pushed < e->cap ? pushed : e->cap);
            if (got > 0)
            {
                e->cap -= got;
                edges[id ^ 1].cap += got;
                return got;
            }
        }
    }
    return 0;
}

int main(void)
{
    int n = 6;
    add_edge(0, 1, 16); add_edge(0, 2, 13); add_edge(1, 2, 10); add_edge(2, 1, 4); add_edge(1, 3, 12);
    add_edge(3, 2, 9); add_edge(2, 4, 14); add_edge(4, 3, 7); add_edge(3, 5, 20); add_edge(4, 5, 4);

    int flow = 0, phases = 0;
    while (bfs(0, 5, n))
    {
        memset(it, 0, sizeof it);
        int f;
        while ((f = dfs(0, 5, 1 << 30)) > 0)
            flow += f;
        phases++;
    }
    printf("maximum flow = %d found in %d BFS phases\n", flow, phases);
    return 0;
}
