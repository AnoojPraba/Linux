// Maximum flow with Edmonds-Karp (Ford-Fulkerson using BFS augmenting paths)
// and the min cut it implies.

#include <stdio.h>
#include <string.h>

#define N 6
static int cap[N][N];

static int bfs(int s, int t, int *parent)
{
    int q[N], h = 0, tl = 0;
    memset(parent, -1, N * sizeof(int));
    parent[s] = s;
    q[tl++] = s;
    while (h < tl)
    {
        int u = q[h++];
        for (int v = 0; v < N; v++)
            if (parent[v] < 0 && cap[u][v] > 0)        /* residual capacity left */
            {
                parent[v] = u;
                if (v == t) return 1;
                q[tl++] = v;
            }
    }
    return 0;
}

int main(void)
{
    cap[0][1] = 16; cap[0][2] = 13; cap[1][2] = 10; cap[2][1] = 4;
    cap[1][3] = 12; cap[3][2] = 9; cap[2][4] = 14; cap[4][3] = 7; cap[3][5] = 20; cap[4][5] = 4;

    int flow = 0, parent[N];
    while (bfs(0, 5, parent))
    {
        int bottleneck = 1 << 30;
        for (int v = 5; v != 0; v = parent[v])
            if (cap[parent[v]][v] < bottleneck) bottleneck = cap[parent[v]][v];
        for (int v = 5; v != 0; v = parent[v])
        {
            cap[parent[v]][v] -= bottleneck;           /* use capacity forward ... */
            cap[v][parent[v]] += bottleneck;           /* ... and open the reverse edge so a choice can be undone */
        }
        flow += bottleneck;
        printf("augmenting path adds %d (total %d)\n", bottleneck, flow);
    }
    printf("maximum flow = %d\n", flow);

    parent[0] = 0;
    bfs(0, 5, parent);                                 /* vertices still reachable in the residual graph = source side of a min cut */
    printf("source side of the minimum cut:");
    for (int v = 0; v < N; v++)
        if (parent[v] >= 0) printf(" %d", v);
    printf("\n");
    return 0;
}
