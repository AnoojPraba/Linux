// Maximum bipartite matching with Kuhn's augmenting-path algorithm (jobs to
// workers), Hopcroft-Karp (BFS layering + DFS, O(E sqrt V)), and the
// matching/vertex-cover relation.

#include <stdio.h>
#include <string.h>

#define L 5          /* left vertices: workers */
#define Rn 5         /* right vertices: jobs */
#define INF_DIST (L + 1)
static int ok[L][Rn];          /* ok[w][j] = worker w can do job j */
static int match_l[L];         /* match_l[w] = job currently held by worker w, or -1 */
static int match_r[Rn];        /* match_r[j] = worker currently holding job j, or -1 */
static int visited[Rn];
static int dist[L];            /* Hopcroft-Karp BFS layer of each left vertex */

static int try_kuhn(int w)
{
    for (int j = 0; j < Rn; j++)
    {
        if (!ok[w][j] || visited[j]) continue;
        visited[j] = 1;
        if (match_r[j] < 0 || try_kuhn(match_r[j]))    /* job is free, or its holder can move to another job */
        {
            match_r[j] = w;
            return 1;
        }
    }
    return 0;
}

/* BFS: layer every free left vertex at 0, layer matched left vertices one
 * step past the right vertex that leads back to them; returns 1 if at
 * least one augmenting path exists this phase. */
static int hk_bfs(void)
{
    int queue[L], head = 0, tail = 0;
    int found_free_right = 0;
    for (int w = 0; w < L; w++)
    {
        if (match_l[w] < 0) { dist[w] = 0; queue[tail++] = w; }
        else dist[w] = INF_DIST;
    }
    while (head < tail)
    {
        int w = queue[head++];
        for (int j = 0; j < Rn; j++)
        {
            if (!ok[w][j]) continue;
            int owner = match_r[j];
            if (owner < 0) { found_free_right = 1; continue; }         /* reached a free job: this phase has an augmenting path */
            if (dist[owner] == INF_DIST) { dist[owner] = dist[w] + 1; queue[tail++] = owner; }
        }
    }
    return found_free_right;
}

/* DFS restricted to edges that advance exactly one BFS layer, so every
 * path found in a phase is a shortest augmenting path. */
static int hk_dfs(int w)
{
    for (int j = 0; j < Rn; j++)
    {
        if (!ok[w][j] || visited[j]) continue;
        int owner = match_r[j];
        if (owner < 0 || (dist[owner] == dist[w] + 1 && hk_dfs(owner)))
        {
            visited[j] = 1;
            match_l[w] = j;
            match_r[j] = w;
            return 1;
        }
    }
    dist[w] = INF_DIST;    /* dead end this phase: don't retry through w again */
    return 0;
}

static int hopcroft_karp(void)
{
    memset(match_l, -1, sizeof match_l);
    memset(match_r, -1, sizeof match_r);
    int size = 0;
    while (hk_bfs())
    {
        memset(visited, 0, sizeof visited);
        for (int w = 0; w < L; w++)
            if (match_l[w] < 0 && hk_dfs(w)) size++;    /* many shortest augmenting paths found per phase */
    }
    return size;
}

int main(void)
{
    int pairs[][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 2}, {2, 1}, {2, 3}, {3, 2}, {3, 3}, {4, 3}};
    for (unsigned i = 0; i < sizeof pairs / sizeof pairs[0]; i++)
        ok[pairs[i][0]][pairs[i][1]] = 1;

    memset(match_r, -1, sizeof match_r);
    int size = 0;
    for (int w = 0; w < L; w++)
    {
        memset(visited, 0, sizeof visited);
        size += try_kuhn(w);
    }
    printf("Kuhn maximum matching = %d\n", size);
    for (int j = 0; j < Rn; j++)
        if (match_r[j] >= 0) printf("  job %d <- worker %d\n", j, match_r[j]);
    printf("by Konig's theorem the minimum vertex cover also has %d vertices\n", size);

    int hk_size = hopcroft_karp();
    printf("Hopcroft-Karp maximum matching = %d\n", hk_size);
    printf("Kuhn O(V E), Hopcroft-Karp O(E sqrt V); the same answer comes from max flow source -> left -> right -> sink\n");
    return 0;
}
