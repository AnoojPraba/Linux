// Eulerian circuit/path with Hierholzer's algorithm and the degree conditions
// for existence.

#include <stdio.h>
#include <string.h>

#define N 6
static int g[N][N];                 /* edge counts of an undirected multigraph */
static int path[64], pn;

static void hierholzer(int u)
{
    for (int v = 0; v < N; v++)
        while (g[u][v] > 0)
        {
            g[u][v]--; g[v][u]--;   /* use the edge once */
            hierholzer(v);
        }
    path[pn++] = u;                 /* vertices are appended on the way back */
}

static void report(const char *what, int start)
{
    int deg[N] = {0}, odd = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) deg[i] += g[i][j];
    for (int i = 0; i < N; i++) odd += deg[i] % 2;
    printf("%s: %d vertices of odd degree -> %s\n", what, odd,
           odd == 0 ? "Eulerian circuit exists" : odd == 2 ? "Eulerian path exists (start at an odd vertex)" : "no Eulerian path");
    pn = 0;
    hierholzer(start);
    printf("  route:");
    for (int i = pn - 1; i >= 0; i--) printf(" %d", path[i]);
    printf("\n");
}

static void add(int a, int b) { g[a][b]++; g[b][a]++; }

int main(void)
{
    add(0, 1); add(1, 2); add(2, 0); add(2, 3); add(3, 4); add(4, 2);        /* two triangles sharing vertex 2 */
    report("bow-tie graph", 0);

    memset(g, 0, sizeof g);
    add(0, 1); add(1, 2); add(2, 3); add(3, 1);                              /* a triangle with a tail: vertices 0 and 3? */
    add(3, 4);
    report("triangle with a tail", 0);
    return 0;
}
