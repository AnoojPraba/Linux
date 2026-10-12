// Longest path in a DAG by DP over a topological order: project scheduling,
// earliest start times and the critical path.

#include <stdio.h>

#define N 7
static const char *name[N] = {"design", "backend", "frontend", "database", "integrate", "test", "release"};
static const int dur[N] = {3, 5, 4, 2, 2, 3, 1};
static int dep[N][N];              /* dep[a][b] = 1: b must wait for a */

int main(void)
{
    dep[0][1] = dep[0][2] = dep[0][3] = 1;
    dep[1][4] = dep[2][4] = dep[3][4] = 1;
    dep[4][5] = 1;
    dep[5][6] = 1;

    int indeg[N] = {0}, order[N], on = 0, q[N], h = 0, t = 0;
    for (int a = 0; a < N; a++)
        for (int b = 0; b < N; b++) indeg[b] += dep[a][b];
    for (int i = 0; i < N; i++)
        if (!indeg[i]) q[t++] = i;
    while (h < t)
    {
        int u = q[h++];
        order[on++] = u;
        for (int v = 0; v < N; v++)
            if (dep[u][v] && --indeg[v] == 0) q[t++] = v;
    }

    int start[N] = {0}, pred[N];
    for (int i = 0; i < N; i++) pred[i] = -1;
    for (int k = 0; k < N; k++)                        /* a task can start when its slowest prerequisite ends */
    {
        int u = order[k];
        for (int v = 0; v < N; v++)
            if (dep[u][v] && start[u] + dur[u] > start[v])
            {
                start[v] = start[u] + dur[u];
                pred[v] = u;
            }
    }
    int last = 0;
    for (int i = 0; i < N; i++)
        if (start[i] + dur[i] > start[last] + dur[last]) last = i;
    printf("earliest start times:\n");
    for (int i = 0; i < N; i++) printf("  %-10s starts at %2d, ends at %2d\n", name[i], start[i], start[i] + dur[i]);
    printf("project length = %d; critical path:", start[last] + dur[last]);
    int chain[N], n = 0;
    for (int v = last; v >= 0; v = pred[v]) chain[n++] = v;
    for (int i = n - 1; i >= 0; i--) printf(" %s", name[chain[i]]);
    printf("\n");
    return 0;
}
