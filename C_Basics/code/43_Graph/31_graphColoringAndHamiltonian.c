// NP-hard graph problems by backtracking: m-colouring, Hamiltonian cycle, plus
// a greedy colouring heuristic.

#include <stdio.h>
#include <string.h>

#define N 6
static int g[N][N];

static void edge(int a, int b) { g[a][b] = g[b][a] = 1; }

/* ---- m-colouring by backtracking ---- */
static int col[N];

static int colour(int v, int m)
{
    if (v == N) return 1;
    for (int c = 1; c <= m; c++)
    {
        int ok = 1;
        for (int u = 0; u < v; u++)
            if (g[v][u] && col[u] == c) { ok = 0; break; }
        if (ok)
        {
            col[v] = c;
            if (colour(v + 1, m)) return 1;
            col[v] = 0;                                /* undo and try the next colour */
        }
    }
    return 0;
}

/* ---- greedy colouring: fast, uses at most max-degree + 1 colours, not always optimal ---- */
static int greedy(void)
{
    int used = 0;
    for (int v = 0; v < N; v++)
    {
        int taken[N + 2] = {0};
        for (int u = 0; u < v; u++)
            if (g[v][u]) taken[col[u]] = 1;
        int c = 1;
        while (taken[c]) c++;
        col[v] = c;
        if (c > used) used = c;
    }
    return used;
}

/* ---- Hamiltonian cycle ---- */
static int path[N], used[N];

static int hamilton(int pos)
{
    if (pos == N) return g[path[N - 1]][path[0]];       /* must close the cycle */
    for (int v = 1; v < N; v++)
        if (!used[v] && g[path[pos - 1]][v])
        {
            used[v] = 1;
            path[pos] = v;
            if (hamilton(pos + 1)) return 1;
            used[v] = 0;
        }
    return 0;
}

int main(void)
{
    edge(0, 1); edge(0, 2); edge(1, 2); edge(1, 3); edge(2, 4); edge(3, 4); edge(3, 5); edge(4, 5);
    int chi = 0;
    for (int m = 1; m <= N; m++)
    {
        memset(col, 0, sizeof col);
        if (colour(0, m)) { chi = m; break; }
    }
    printf("chromatic number = %d, colouring:", chi);
    for (int i = 0; i < N; i++) printf(" %d", col[i]);
    memset(col, 0, sizeof col);
    printf("\ngreedy colouring uses %d colours\n", greedy());

    path[0] = 0; used[0] = 1;
    if (hamilton(1))
    {
        printf("Hamiltonian cycle:");
        for (int i = 0; i < N; i++) printf(" %d", path[i]);
        printf(" %d\n", path[0]);
    }
    else
        printf("no Hamiltonian cycle\n");
    return 0;
}
