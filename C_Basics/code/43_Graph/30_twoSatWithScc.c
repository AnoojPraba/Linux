// 2-SAT solved with strongly connected components of the implication graph
// (Tarjan), including the satisfying assignment.

#include <stdio.h>
#include <string.h>

#define VARS 4
#define NODES (2 * VARS)                     /* node 2*i = x_i, node 2*i+1 = NOT x_i */
static int g[NODES][NODES];

static int idx[NODES], low[NODES], onstk[NODES], stk[NODES], sp, counter, comp[NODES], nc;

static void tarjan(int u)
{
    idx[u] = low[u] = ++counter;
    stk[sp++] = u;
    onstk[u] = 1;
    for (int v = 0; v < NODES; v++)
        if (g[u][v])
        {
            if (!idx[v]) { tarjan(v); if (low[v] < low[u]) low[u] = low[v]; }
            else if (onstk[v] && idx[v] < low[u]) low[u] = idx[v];
        }
    if (low[u] == idx[u])
    {
        int v;
        do { v = stk[--sp]; onstk[v] = 0; comp[v] = nc; } while (v != u);
        nc++;
    }
}

/* clause (a OR b): NOT a -> b and NOT b -> a. Literal encoding: var i true = 2i, false = 2i+1. */
static void clause(int a, int b)
{
    g[a ^ 1][b] = 1;
    g[b ^ 1][a] = 1;
}

int main(void)
{
    enum { X0 = 0, NX0 = 1, X1 = 2, NX1 = 3, X2 = 4, NX2 = 5, X3 = 6, NX3 = 7 };
    clause(X0, X1);          /* x0 or x1       */
    clause(NX0, X2);         /* not x0 or x2   */
    clause(NX1, NX2);        /* not x1 or not x2 */
    clause(X2, X3);          /* x2 or x3       */
    clause(NX3, X0);         /* not x3 or x0   */

    for (int i = 0; i < NODES; i++)
        if (!idx[i]) tarjan(i);

    int sat = 1;
    for (int i = 0; i < VARS; i++)
        if (comp[2 * i] == comp[2 * i + 1]) sat = 0;         /* x and NOT x in one SCC: contradiction */
    printf("satisfiable: %d\n", sat);
    if (sat)
        for (int i = 0; i < VARS; i++)
            /* Tarjan numbers components in reverse topological order: x is true if its component comes later in topological order */
            printf("  x%d = %d\n", i, comp[2 * i] < comp[2 * i + 1]);

    clause(NX0, NX0);        /* force x0 false (not x0 or not x0) */
    clause(X0, X0);          /* ... and true: unsatisfiable */
    memset(idx, 0, sizeof idx); memset(onstk, 0, sizeof onstk); sp = counter = nc = 0;
    for (int i = 0; i < NODES; i++)
        if (!idx[i]) tarjan(i);
    sat = 1;
    for (int i = 0; i < VARS; i++)
        if (comp[2 * i] == comp[2 * i + 1]) sat = 0;
    printf("after forcing x0 both ways: satisfiable: %d\n", sat);
    return 0;
}
