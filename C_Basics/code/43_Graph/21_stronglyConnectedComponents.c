// Strongly connected components with Kosaraju (two passes, transpose) and
// Tarjan (one pass, low-link values).

#include <stdio.h>
#include <string.h>

#define N 8
static int g[N][N], gt[N][N];            /* adjacency matrices of the graph and its transpose */

/* ---- Kosaraju: order by finish time, then DFS on the transpose ---- */
static int seen[N], order[N], on;

static void dfs1(int u)
{
    seen[u] = 1;
    for (int v = 0; v < N; v++)
        if (g[u][v] && !seen[v])
            dfs1(v);
    order[on++] = u;                     /* post-order */
}

static void dfs2(int u, int c, int *comp)
{
    comp[u] = c;
    for (int v = 0; v < N; v++)
        if (gt[u][v] && comp[v] < 0)
            dfs2(v, c, comp);
}

static int kosaraju(int *comp)
{
    memset(seen, 0, sizeof seen);
    on = 0;
    for (int i = 0; i < N; i++)
        if (!seen[i]) dfs1(i);
    for (int i = 0; i < N; i++) comp[i] = -1;
    int c = 0;
    for (int i = N - 1; i >= 0; i--)     /* decreasing finish time */
        if (comp[order[i]] < 0)
            dfs2(order[i], c++, comp);
    return c;
}

/* ---- Tarjan: low-link values and a stack ---- */
static int idx[N], low[N], onstack[N], stack_[N], sp, counter, tcomp[N], tc;

static void tarjan(int u)
{
    idx[u] = low[u] = ++counter;
    stack_[sp++] = u;
    onstack[u] = 1;
    for (int v = 0; v < N; v++)
    {
        if (!g[u][v]) continue;
        if (!idx[v])
        {
            tarjan(v);
            if (low[v] < low[u]) low[u] = low[v];
        }
        else if (onstack[v] && idx[v] < low[u])
            low[u] = idx[v];             /* back edge into the current component */
    }
    if (low[u] == idx[u])                /* u is the root of a component: pop it */
    {
        int v;
        do
        {
            v = stack_[--sp];
            onstack[v] = 0;
            tcomp[v] = tc;
        } while (v != u);
        tc++;
    }
}

static void show(const char *name, const int *comp, int count)
{
    printf("%s found %d components:\n", name, count);
    for (int c = 0; c < count; c++)
    {
        printf("  {");
        int first = 1;
        for (int v = 0; v < N; v++)
            if (comp[v] == c) { printf(first ? "%d" : ", %d", v); first = 0; }
        printf("}\n");
    }
}

int main(void)
{
    int e[][2] = {{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}, {4, 5}, {5, 3}, {6, 5}, {6, 7}, {7, 6}};
    for (unsigned i = 0; i < sizeof e / sizeof e[0]; i++)
    {
        g[e[i][0]][e[i][1]] = 1;
        gt[e[i][1]][e[i][0]] = 1;
    }
    int comp[N];
    show("Kosaraju", comp, kosaraju(comp));
    for (int i = 0; i < N; i++)
        if (!idx[i]) tarjan(i);
    show("Tarjan", tcomp, tc);
    return 0;
}
