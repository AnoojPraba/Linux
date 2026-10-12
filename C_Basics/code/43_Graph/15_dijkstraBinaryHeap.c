// Dijkstra with a binary min-heap (O((V+E) log V)) and shortest-path
// reconstruction through parent pointers.

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXV 8
typedef struct { int to, w; } Edge;
static Edge adj[MAXV][MAXV];
static int deg[MAXV];

typedef struct { int dist, v; } Item;
static Item heap[64];
static int hn;

static void push(Item x)
{
    int i = hn++;
    heap[i] = x;
    while (i > 0 && heap[(i - 1) / 2].dist > heap[i].dist)
    {
        Item t = heap[i]; heap[i] = heap[(i - 1) / 2]; heap[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

static Item pop(void)
{
    Item top = heap[0];
    heap[0] = heap[--hn];
    for (int i = 0;;)
    {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < hn && heap[l].dist < heap[m].dist) m = l;
        if (r < hn && heap[r].dist < heap[m].dist) m = r;
        if (m == i) break;
        Item t = heap[i]; heap[i] = heap[m]; heap[m] = t;
        i = m;
    }
    return top;
}

static void edge(int a, int b, int w)
{
    adj[a][deg[a]++] = (Edge){b, w};
    adj[b][deg[b]++] = (Edge){a, w};
}

int main(void)
{
    edge(0, 1, 4); edge(0, 2, 1); edge(2, 1, 2); edge(1, 3, 1); edge(2, 3, 5); edge(3, 4, 3); edge(5, 6, 1);

    int dist[MAXV], parent[MAXV];
    for (int i = 0; i < MAXV; i++) { dist[i] = INT_MAX; parent[i] = -1; }
    dist[0] = 0;
    push((Item){0, 0});
    while (hn)
    {
        Item it = pop();
        if (it.dist > dist[it.v])
            continue;                                  /* stale entry: a shorter path was already settled */
        for (int i = 0; i < deg[it.v]; i++)
        {
            Edge e = adj[it.v][i];
            if (it.dist + e.w < dist[e.to])
            {
                dist[e.to] = it.dist + e.w;
                parent[e.to] = it.v;
                push((Item){dist[e.to], e.to});
            }
        }
    }
    for (int v = 0; v < 7; v++)
    {
        if (dist[v] == INT_MAX) { printf("0 -> %d: unreachable\n", v); continue; }
        int path[MAXV], n = 0;
        for (int x = v; x >= 0; x = parent[x]) path[n++] = x;
        printf("0 -> %d: distance %d, path", v, dist[v]);
        for (int i = n - 1; i >= 0; i--) printf(" %d", path[i]);
        printf("\n");
    }
    return 0;
}
