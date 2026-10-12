// Girth (length of the shortest cycle) of an undirected graph by running BFS
// from every vertex.

#include <stdio.h>

#define N 8
static int adj[N][N], deg[N];

static void edge(int a, int b) { adj[a][deg[a]++] = b; adj[b][deg[b]++] = a; }

int main(void)
{
    edge(0, 1); edge(1, 2); edge(2, 3); edge(3, 4); edge(4, 5); edge(5, 0);     /* a 6-cycle */
    edge(2, 6); edge(6, 7); edge(7, 3);                                         /* with a chord path forming a 4-cycle 2-3-7-6 */

    int best = 1 << 30;
    for (int s = 0; s < N; s++)
    {
        int dist[N], parent[N], q[N], h = 0, t = 0;
        for (int i = 0; i < N; i++) dist[i] = -1;
        dist[s] = 0; parent[s] = -1; q[t++] = s;
        while (h < t)
        {
            int u = q[h++];
            for (int i = 0; i < deg[u]; i++)
            {
                int v = adj[u][i];
                if (dist[v] < 0)
                {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q[t++] = v;
                }
                else if (v != parent[u] && dist[u] + dist[v] + 1 < best)
                    best = dist[u] + dist[v] + 1;      /* a non-tree edge closes a cycle through the BFS tree */
            }
        }
    }
    if (best == 1 << 30) printf("no cycle (a forest)\n");
    else printf("girth (shortest cycle) = %d\n", best);
    return 0;
}
