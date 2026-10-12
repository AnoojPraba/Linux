// Johnson's all-pairs shortest paths for sparse graphs with negative edges:
// Bellman-Ford reweighting, then Dijkstra from every vertex.

#include <limits.h>
#include <stdio.h>

#define N 4
#define INF 1000000
typedef struct { int u, v, w; } Edge;

int main(void)
{
    Edge e[] = {{0, 1, -2}, {1, 2, 3}, {2, 0, 4}, {0, 3, 2}, {3, 2, -1}};
    int m = sizeof e / sizeof e[0];

    /* 1. add a virtual source N with 0-weight edges to every vertex and run Bellman-Ford to get potentials h[v] */
    int h[N + 1];
    for (int i = 0; i <= N; i++) h[i] = INF;
    h[N] = 0;
    for (int pass = 0; pass <= N; pass++)
    {
        for (int i = 0; i < N; i++)
            if (h[N] + 0 < h[i]) h[i] = 0;
        for (int i = 0; i < m; i++)
            if (h[e[i].u] < INF && h[e[i].u] + e[i].w < h[e[i].v])
            {
                h[e[i].v] = h[e[i].u] + e[i].w;
                if (pass == N) { printf("negative cycle\n"); return 1; }
            }
    }
    printf("potentials h:");
    for (int i = 0; i < N; i++) printf(" %d", h[i]);
    printf("\n");

    /* 2. reweight: w'(u,v) = w + h[u] - h[v] >= 0 for every edge, and shortest paths keep their shape */
    int rw[8];
    for (int i = 0; i < m; i++)
    {
        rw[i] = e[i].w + h[e[i].u] - h[e[i].v];
        if (rw[i] < 0) { printf("reweighting failed\n"); return 1; }
    }

    /* 3. Dijkstra from each vertex (O(V^2) scan version), then undo the reweighting */
    printf("all-pairs shortest distances (row = from):\n");
    for (int s = 0; s < N; s++)
    {
        int d[N], done[N] = {0};
        for (int i = 0; i < N; i++) d[i] = INF;
        d[s] = 0;
        for (int it = 0; it < N; it++)
        {
            int u = -1;
            for (int i = 0; i < N; i++)
                if (!done[i] && (u < 0 || d[i] < d[u])) u = i;
            if (d[u] >= INF) break;
            done[u] = 1;
            for (int i = 0; i < m; i++)
                if (e[i].u == u && d[u] + rw[i] < d[e[i].v]) d[e[i].v] = d[u] + rw[i];
        }
        printf("  %d:", s);
        for (int t = 0; t < N; t++)
            if (d[t] >= INF) printf("  INF");
            else printf(" %4d", d[t] - h[s] + h[t]);   /* undo: dist(s,t) = d'(s,t) - h[s] + h[t] */
        printf("\n");
    }
    return 0;
}
