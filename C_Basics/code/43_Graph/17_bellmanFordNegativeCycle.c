// Bellman-Ford that reports a reachable negative cycle and prints the cycle
// itself by walking parent pointers.

#include <stdio.h>

#define N 5
typedef struct { int u, v, w; } Edge;

int main(void)
{
    Edge e[] = {{0, 1, 1}, {1, 2, -1}, {2, 3, -1}, {3, 1, -1}, {3, 4, 2}};   /* 1 -> 2 -> 3 -> 1 sums to -3 */
    int m = sizeof e / sizeof e[0];
    int dist[N] = {0}, parent[N];
    for (int i = 0; i < N; i++) parent[i] = -1;

    int last = -1;
    for (int pass = 0; pass < N; pass++)               /* V passes: a relaxation in the V-th pass proves a cycle */
    {
        last = -1;
        for (int i = 0; i < m; i++)
            if (dist[e[i].u] + e[i].w < dist[e[i].v])
            {
                dist[e[i].v] = dist[e[i].u] + e[i].w;
                parent[e[i].v] = e[i].u;
                last = e[i].v;
            }
        if (last < 0) break;
    }

    if (last < 0)
    {
        printf("no negative cycle\n");
        return 0;
    }
    int x = last;
    for (int i = 0; i < N; i++) x = parent[x];         /* step back V times to be certain x is ON the cycle */
    int cycle[N], n = 0, v = x;
    do { cycle[n++] = v; v = parent[v]; } while (v != x);
    printf("negative cycle:");
    for (int i = n - 1; i >= 0; i--) printf(" %d", cycle[i]);
    printf(" %d\n", cycle[n - 1]);
    return 0;
}
