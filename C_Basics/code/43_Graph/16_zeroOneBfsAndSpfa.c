// 0-1 BFS with a deque for edges of weight 0 or 1, and SPFA (queue-based
// Bellman-Ford) for graphs with negative edges.

#include <stdio.h>
#include <string.h>

#define N 6
typedef struct { int to, w; } Edge;
static Edge adj[N][N];
static int deg[N];

static void add(int a, int b, int w) { adj[a][deg[a]++] = (Edge){b, w}; }

/* deque as a circular array: push_front for weight 0, push_back for weight 1 */
#define DQ 64
static int dq[DQ], head, tail, cnt;
static void push_front(int x) { head = (head + DQ - 1) % DQ; dq[head] = x; cnt++; }
static void push_back(int x) { dq[tail] = x; tail = (tail + 1) % DQ; cnt++; }
static int pop_front(void) { int x = dq[head]; head = (head + 1) % DQ; cnt--; return x; }

static void zero_one_bfs(int src, int *dist)
{
    for (int i = 0; i < N; i++) dist[i] = 1 << 30;
    dist[src] = 0;
    head = tail = cnt = 0;
    push_back(src);
    while (cnt)
    {
        int u = pop_front();
        for (int i = 0; i < deg[u]; i++)
        {
            Edge e = adj[u][i];
            if (dist[u] + e.w < dist[e.to])
            {
                dist[e.to] = dist[u] + e.w;
                if (e.w == 0) push_front(e.to); else push_back(e.to);   /* keeps the deque ordered by distance */
            }
        }
    }
}

static int spfa(int src, int *dist)
{
    int inq[N] = {0}, relax[N] = {0};
    for (int i = 0; i < N; i++) dist[i] = 1 << 30;
    dist[src] = 0;
    head = tail = cnt = 0;
    push_back(src); inq[src] = 1;
    while (cnt)
    {
        int u = pop_front();
        inq[u] = 0;
        for (int i = 0; i < deg[u]; i++)
        {
            Edge e = adj[u][i];
            if (dist[u] + e.w < dist[e.to])
            {
                dist[e.to] = dist[u] + e.w;
                if (++relax[e.to] > N) return 0;       /* relaxed more than V times: a negative cycle */
                if (!inq[e.to]) { push_back(e.to); inq[e.to] = 1; }
            }
        }
    }
    return 1;
}

int main(void)
{
    int d[N];
    add(0, 1, 0); add(1, 2, 1); add(0, 2, 1); add(2, 3, 0); add(3, 4, 1); add(1, 4, 1);
    zero_one_bfs(0, d);
    printf("0-1 BFS distances from 0:");
    for (int i = 0; i < 5; i++) printf(" %d", d[i]);
    printf("\n");

    memset(deg, 0, sizeof deg);
    add(0, 1, 4); add(0, 2, 5); add(1, 2, -3); add(2, 3, 4); add(1, 3, 6);
    int ok = spfa(0, d);
    printf("SPFA with a negative edge, no negative cycle: %d, distances:", ok);
    for (int i = 0; i < 4; i++) printf(" %d", d[i]);
    printf("\n");

    add(3, 1, -2);                                     /* 1 -> 2 -> 3 -> 1 now has weight -3 + 4 - 2 = -1 */
    printf("after adding 3 -> 1 (-2): negative cycle detected: %d\n", !spfa(0, d));
    return 0;
}
