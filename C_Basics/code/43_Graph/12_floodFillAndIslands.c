// Grid graphs: flood fill, counting islands (DFS), largest island, and
// counting with union-find.

#include <stdio.h>

#define R 5
#define C 6
static char g[R][C + 1] = {
    "11000.",
    "11000.",
    "00100.",
    "0001.1",
    "00011."};

static int fill(int r, int c)
{
    if (r < 0 || r >= R || c < 0 || c >= C || g[r][c] != '1') return 0;
    g[r][c] = '#';                                     /* mark visited by overwriting */
    return 1 + fill(r + 1, c) + fill(r - 1, c) + fill(r, c + 1) + fill(r, c - 1);
}

static int parent[R * C];
static int find(int x) { while (parent[x] != x) x = parent[x] = parent[parent[x]]; return x; }

int main(void)
{
    /* union-find version first, on a pristine copy of the grid */
    for (int i = 0; i < R * C; i++) parent[i] = i;
    int comps = 0;
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++)
            if (g[r][c] == '1')
            {
                comps++;
                if (r + 1 < R && g[r + 1][c] == '1' && find(r * C + c) != find((r + 1) * C + c)) { parent[find(r * C + c)] = find((r + 1) * C + c); comps--; }
                if (c + 1 < C && g[r][c + 1] == '1' && find(r * C + c) != find(r * C + c + 1)) { parent[find(r * C + c)] = find(r * C + c + 1); comps--; }
            }
    printf("islands by union-find: %d\n", comps);

    int islands = 0, biggest = 0;
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++)
            if (g[r][c] == '1')
            {
                int size = fill(r, c);                 /* 4-directional flood fill */
                islands++;
                if (size > biggest) biggest = size;
            }
    printf("islands by DFS flood fill: %d, largest has %d cells\n", islands, biggest);
    for (int r = 0; r < R; r++) printf("  %s\n", g[r]);
    return 0;
}
