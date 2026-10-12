// Minimum-cost maximum flow with successive shortest paths (SPFA on the
// residual graph): assignment and transport problems.

#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAXN 10
#define MAXE 80     /* each add_edge call adds a forward and a reverse edge */

typedef struct { int to, cap, cost; } Edge;

static Edge edges[MAXE];
static int edge_count;
static int adj[MAXN][MAXE];
static int adj_len[MAXN];
static int n_nodes;

static void add_edge(int a, int b, int cap, int cost)
{
    adj[a][adj_len[a]++] = edge_count; edges[edge_count++] = (Edge){b, cap, cost};
    adj[b][adj_len[b]++] = edge_count; edges[edge_count++] = (Edge){a, 0, -cost};   /* reverse edge refunds the cost */
}

/* One successive-shortest-path augmentation; returns the flow pushed this
 * round (0 when s cannot reach t in the residual graph). *cost_out
 * accumulates flow * path_cost. */
static int spfa_augment(int s, int t, int *cost_inout)
{
    int dist[MAXN], prev_edge[MAXN], in_queue[MAXN];
    int queue[MAXN * MAXN], head = 0, tail = 0;
    for (int i = 0; i < n_nodes; i++) { dist[i] = INT_MAX / 2; prev_edge[i] = -1; in_queue[i] = 0; }
    dist[s] = 0;
    queue[tail++] = s; in_queue[s] = 1;
    while (head < tail)                                            /* SPFA: negative costs appear on reverse edges, so no plain Dijkstra */
    {
        int u = queue[head++]; in_queue[u] = 0;
        for (int k = 0; k < adj_len[u]; k++)
        {
            int id = adj[u][k];
            if (edges[id].cap > 0 && dist[u] + edges[id].cost < dist[edges[id].to])
            {
                dist[edges[id].to] = dist[u] + edges[id].cost;
                prev_edge[edges[id].to] = id;
                if (!in_queue[edges[id].to]) { in_queue[edges[id].to] = 1; queue[tail++] = edges[id].to; }
            }
        }
    }
    if (dist[t] >= INT_MAX / 2) return 0;

    int push = INT_MAX;
    for (int v = t; v != s; v = edges[prev_edge[v] ^ 1].to)
        if (edges[prev_edge[v]].cap < push) push = edges[prev_edge[v]].cap;
    for (int v = t; v != s; v = edges[prev_edge[v] ^ 1].to)
    {
        edges[prev_edge[v]].cap -= push;
        edges[prev_edge[v] ^ 1].cap += push;
    }
    *cost_inout += push * dist[t];
    return push;
}

static int min_cost_max_flow(int s, int t, int *total_cost)
{
    int flow = 0;
    *total_cost = 0;
    int pushed;
    while ((pushed = spfa_augment(s, t, total_cost)) > 0) flow += pushed;
    return flow;
}

static void reset_graph(int n)
{
    n_nodes = n;
    edge_count = 0;
    memset(adj_len, 0, sizeof adj_len);
}

int main(void)
{
    /* assignment problem: 3 workers, 3 jobs, cost matrix; source 0, workers
     * 1..3, jobs 4..6, sink 7 */
    int cost[3][3] = {{9, 2, 7}, {6, 4, 3}, {5, 8, 1}};
    reset_graph(8);
    for (int w = 0; w < 3; w++) add_edge(0, 1 + w, 1, 0);
    for (int j = 0; j < 3; j++) add_edge(4 + j, 7, 1, 0);
    for (int w = 0; w < 3; w++)
        for (int j = 0; j < 3; j++) add_edge(1 + w, 4 + j, 1, cost[w][j]);
    int total = 0;
    int flow = min_cost_max_flow(0, 7, &total);
    printf("assignment: %d workers assigned, minimum total cost %d\n", flow, total);

    /* transport: 0 -> {1, 2} -> 3, capacities and per-unit costs */
    reset_graph(4);
    add_edge(0, 1, 4, 1);
    add_edge(0, 2, 2, 2);
    add_edge(1, 2, 2, 1);
    add_edge(1, 3, 3, 3);
    add_edge(2, 3, 5, 1);
    int total2 = 0;
    int flow2 = min_cost_max_flow(0, 3, &total2);
    printf("transport network: max flow %d at minimum cost %d\n", flow2, total2);
    printf("successive shortest paths: augment along the cheapest path each time; O(F * SPFA); Hungarian algorithm is faster for pure assignment\n");
    return 0;
}
