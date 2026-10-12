// DFS colour cycle detection on a wait-for graph, abort the lowest-priority
// process in the cycle, re-check.

#include <stdio.h>

#define N 5

static int waits[N][N];                           /* waits[a][b]: process a waits for a resource held by b */
static const int priority[N] = {5, 2, 8, 3, 6};   /* a higher number is more important */
static int colour[N], path[N], depth, cycle[N], cycle_len;

static int dfs(int u)
{
    colour[u] = 1;
    path[depth++] = u;
    for (int v = 0; v < N; v++)
        if (waits[u][v])
        {
            if (colour[v] == 1)                    /* back edge: the cycle is the path from v to u */
            {
                int s = 0;
                while (path[s] != v) s++;
                cycle_len = 0;
                for (int i = s; i < depth; i++) cycle[cycle_len++] = path[i];
                return 1;
            }
            if (colour[v] == 0 && dfs(v))
                return 1;
        }
    depth--;
    colour[u] = 2;
    return 0;
}

static int find_cycle(void)
{
    for (int i = 0; i < N; i++) colour[i] = 0;
    depth = 0;
    for (int u = 0; u < N; u++)
        if (colour[u] == 0 && dfs(u))
            return 1;
    return 0;
}

int main(void)
{
    waits[0][1] = waits[1][2] = waits[2][3] = waits[3][1] = 1;   /* P1 -> P2 -> P3 -> P1 is a cycle */
    waits[4][0] = 1;                                              /* P4 waits for P0, not in a cycle */

    while (find_cycle())
    {
        printf("deadlock among:");
        int victim = cycle[0];
        for (int i = 0; i < cycle_len; i++)
        {
            printf(" P%d(prio %d)", cycle[i], priority[cycle[i]]);
            if (priority[cycle[i]] < priority[victim])
                victim = cycle[i];
        }
        printf("\nabort victim P%d: its resources are released\n", victim);
        for (int j = 0; j < N; j++)                    /* nobody waits for the victim's resources any more */
        {
            waits[j][victim] = 0;
            waits[victim][j] = 0;
        }
    }
    printf("no deadlock remains\n");
    return 0;
}
