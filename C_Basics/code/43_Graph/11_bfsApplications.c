// BFS applications: shortest path with reconstruction, grid maze, bipartite
// test, multi-source BFS and level counting.

#include <stdio.h>
#include <string.h>

#define MAXV 10
static int adj[MAXV][MAXV], deg[MAXV];

static void edge(int u, int v) { adj[u][deg[u]++] = v; adj[v][deg[v]++] = u; }

static int bfs_path(int n, int src, int dst, int *path)
{
    int dist[MAXV], parent[MAXV], q[MAXV], h = 0, t = 0;
    for (int i = 0; i < n; i++) dist[i] = -1;
    dist[src] = 0; parent[src] = -1; q[t++] = src;
    while (h < t)
    {
        int u = q[h++];
        for (int i = 0; i < deg[u]; i++)
        {
            int v = adj[u][i];
            if (dist[v] < 0)
            {
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q[t++] = v;
            }
        }
    }
    if (dist[dst] < 0) return -1;
    int len = dist[dst] + 1;
    for (int v = dst, k = len - 1; v >= 0; v = parent[v], k--) path[k] = v;   /* walk the parents back */
    return len;
}

static int bipartite(int n)
{
    int side[MAXV], q[MAXV];
    for (int i = 0; i < n; i++) side[i] = -1;
    for (int s = 0; s < n; s++)
    {
        if (side[s] >= 0) continue;
        int h = 0, t = 0;
        side[s] = 0; q[t++] = s;
        while (h < t)
        {
            int u = q[h++];
            for (int i = 0; i < deg[u]; i++)
            {
                int v = adj[u][i];
                if (side[v] < 0) { side[v] = !side[u]; q[t++] = v; }
                else if (side[v] == side[u]) return 0;   /* an odd cycle */
            }
        }
    }
    return 1;
}

/* ---- grid: '.' open, '#' wall; shortest number of steps from S to E ---- */
#define R 5
#define C 7
static const char *maze[R] = {
    "S..#...",
    ".#.#.#.",
    ".#...#.",
    ".####.E",
    "......."};

static int grid_bfs(void)
{
    int dist[R][C], qr[R * C], qc[R * C], h = 0, t = 0, sr = 0, sc = 0, er = 0, ec = 0;
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++)
        {
            dist[r][c] = -1;
            if (maze[r][c] == 'S') { sr = r; sc = c; }
            if (maze[r][c] == 'E') { er = r; ec = c; }
        }
    dist[sr][sc] = 0; qr[t] = sr; qc[t++] = sc;
    static const int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    while (h < t)
    {
        int r = qr[h], c = qc[h++];
        for (int k = 0; k < 4; k++)
        {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr >= 0 && nr < R && nc >= 0 && nc < C && maze[nr][nc] != '#' && dist[nr][nc] < 0)
            {
                dist[nr][nc] = dist[r][c] + 1;
                qr[t] = nr; qc[t++] = nc;
            }
        }
    }
    return dist[er][ec];
}

/* ---- multi-source BFS: time for a "rot" to spread from all rotten cells (2) over fresh cells (1) ---- */
static int rotting(void)
{
    int g[3][4] = {{2, 1, 1, 0}, {1, 1, 0, 1}, {0, 1, 1, 2}};
    int qr[12], qc[12], h = 0, t = 0, fresh = 0, minutes = 0;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 4; c++)
        {
            if (g[r][c] == 2) { qr[t] = r; qc[t++] = c; }   /* every source starts in the queue at distance 0 */
            if (g[r][c] == 1) fresh++;
        }
    static const int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    while (h < t && fresh)
    {
        int end = t;
        while (h < end)                                     /* process one whole level = one minute */
        {
            int r = qr[h], c = qc[h++];
            for (int k = 0; k < 4; k++)
            {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr >= 0 && nr < 3 && nc >= 0 && nc < 4 && g[nr][nc] == 1)
                {
                    g[nr][nc] = 2; fresh--;
                    qr[t] = nr; qc[t++] = nc;
                }
            }
        }
        minutes++;
    }
    return fresh ? -1 : minutes;
}

int main(void)
{
    edge(0, 1); edge(0, 2); edge(1, 3); edge(2, 3); edge(3, 4); edge(4, 5);
    int path[MAXV];
    int len = bfs_path(6, 0, 5, path);
    printf("shortest path 0 -> 5 has %d edges:", len - 1);
    for (int i = 0; i < len; i++) printf(" %d", path[i]);
    printf("\nsquare 0-1-3-2 plus a tail is bipartite: %d\n", bipartite(6));
    edge(3, 5);                                    /* 3-4-5 becomes a triangle: odd cycle */
    printf("after adding 3-5 (triangle 3-4-5): %d\n", bipartite(6));
    printf("maze: shortest way from S to E takes %d steps\n", grid_bfs());
    printf("multi-source BFS: everything rots after %d minutes\n", rotting());
    return 0;
}
