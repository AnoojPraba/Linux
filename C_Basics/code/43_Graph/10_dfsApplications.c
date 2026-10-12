// DFS applications: connected components, cycle detection in undirected and
// directed graphs, entry/exit times and edge classification.

#include <stdio.h>
#include <string.h>

#define MAXV 12

static int adj[MAXV][MAXV], deg[MAXV];       /* adjacency list in arrays */

static void edge(int u, int v, int directed)
{
    adj[u][deg[u]++] = v;
    if (!directed)
        adj[v][deg[v]++] = u;
}

static void reset(void) { memset(deg, 0, sizeof deg); }

/* ---- connected components of an undirected graph ---- */
static void mark(int u, int *comp, int id)
{
    comp[u] = id;
    for (int i = 0; i < deg[u]; i++)
        if (comp[adj[u][i]] < 0)
            mark(adj[u][i], comp, id);
}

static int components(int n, int *comp)
{
    int id = 0;
    for (int i = 0; i < n; i++) comp[i] = -1;
    for (int i = 0; i < n; i++)
        if (comp[i] < 0)
            mark(i, comp, id++);
    return id;
}

/* ---- cycle in an undirected graph: an edge to a visited vertex that is not the parent ---- */
static int undirected_cycle(int u, int parent, int *seen)
{
    seen[u] = 1;
    for (int i = 0; i < deg[u]; i++)
    {
        int v = adj[u][i];
        if (!seen[v])
        {
            if (undirected_cycle(v, u, seen))
                return 1;
        }
        else if (v != parent)
            return 1;
    }
    return 0;
}

/* ---- cycle in a directed graph: an edge to a vertex that is still on the DFS path (grey) ---- */
static int directed_cycle(int u, int *colour)
{
    colour[u] = 1;
    for (int i = 0; i < deg[u]; i++)
    {
        int v = adj[u][i];
        if (colour[v] == 1 || (colour[v] == 0 && directed_cycle(v, colour)))
            return 1;
    }
    colour[u] = 2;
    return 0;
}

/* ---- timestamps and edge classification in a directed graph ---- */
static int tin[MAXV], tout[MAXV], timer_;

static void classify(int u)
{
    tin[u] = ++timer_;
    for (int i = 0; i < deg[u]; i++)
    {
        int v = adj[u][i];
        if (!tin[v])
        {
            printf("  tree edge    %d -> %d\n", u, v);
            classify(v);
        }
        else if (!tout[v])
            printf("  back edge    %d -> %d (v is an ancestor: cycle)\n", u, v);
        else if (tin[u] < tin[v])
            printf("  forward edge %d -> %d\n", u, v);
        else
            printf("  cross edge   %d -> %d\n", u, v);
    }
    tout[u] = ++timer_;
}

int main(void)
{
    int comp[MAXV];
    edge(0, 1, 0); edge(1, 2, 0); edge(3, 4, 0); edge(5, 6, 0);
    printf("undirected graph with 7 vertices has %d connected components\n", components(7, comp));
    for (int i = 0; i < 7; i++) printf("  vertex %d in component %d\n", i, comp[i]);

    int seen[MAXV] = {0};
    printf("cycle in the undirected forest: %d\n", undirected_cycle(0, -1, seen));
    edge(2, 0, 0);                              /* closes the triangle 0-1-2 */
    memset(seen, 0, sizeof seen);
    printf("after adding edge 2-0: %d\n", undirected_cycle(0, -1, seen));

    reset();
    edge(0, 1, 1); edge(0, 2, 1); edge(1, 3, 1); edge(2, 3, 1); edge(3, 1, 1);
    int colour[MAXV] = {0};
    printf("directed graph has a cycle (3 -> 1 -> 3): %d\n", directed_cycle(0, colour));

    printf("edge classification from vertex 0:\n");
    classify(0);
    return 0;
}
