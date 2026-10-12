// Maximum bipartite matching with Kuhn's augmenting-path algorithm (jobs to
// workers) and the matching/vertex-cover relation.

#include <stdio.h>
#include <string.h>

#define L 5          /* left vertices: workers */
#define Rn 5         /* right vertices: jobs */
static int ok[L][Rn];          /* ok[w][j] = worker w can do job j */
static int match_r[Rn];        /* match_r[j] = worker currently holding job j, or -1 */
static int visited[Rn];

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
    printf("maximum matching size = %d\n", size);
    for (int j = 0; j < Rn; j++)
        if (match_r[j] >= 0) printf("  job %d <- worker %d\n", j, match_r[j]);
    printf("by Konig's theorem the minimum vertex cover also has %d vertices\n", size);
    return 0;
}
