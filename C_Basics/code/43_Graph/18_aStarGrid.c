// A* search on a grid with the Manhattan-distance heuristic; compares nodes
// expanded with plain Dijkstra/BFS.

#include <stdio.h>
#include <stdlib.h>

#define R 8
#define C 10
static const char *grid[R] = {
    "S.........",
    ".########.",
    ".#......#.",
    ".#.####.#.",
    ".#.#..#.#.",
    ".#.#.##.#.",
    ".#.#....#E",
    "...#######"};

typedef struct { int f, g, r, c; } Node;
static Node open_[R * C * 4];
static int on;
static int best_g[R][C];

static void push(Node n)
{
    int i = on++;
    open_[i] = n;
    while (i > 0 && open_[(i - 1) / 2].f > open_[i].f)
    {
        Node t = open_[i]; open_[i] = open_[(i - 1) / 2]; open_[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

static Node pop(void)
{
    Node top = open_[0];
    open_[0] = open_[--on];
    for (int i = 0;;)
    {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < on && open_[l].f < open_[m].f) m = l;
        if (r < on && open_[r].f < open_[m].f) m = r;
        if (m == i) break;
        Node t = open_[i]; open_[i] = open_[m]; open_[m] = t;
        i = m;
    }
    return top;
}

static int search(int use_heuristic, int *expanded)
{
    int er = 6, ec = 9;                                    /* the exit E */
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++)
            best_g[r][c] = 1 << 30;
    on = 0;
    *expanded = 0;
    best_g[0][0] = 0;
    push((Node){0, 0, 0, 0});
    static const int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    while (on)
    {
        Node n = pop();
        if (n.g > best_g[n.r][n.c]) continue;
        (*expanded)++;
        if (n.r == er && n.c == ec) return n.g;
        for (int k = 0; k < 4; k++)
        {
            int nr = n.r + dr[k], nc = n.c + dc[k];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C || grid[nr][nc] == '#') continue;
            int g = n.g + 1;
            if (g < best_g[nr][nc])
            {
                best_g[nr][nc] = g;
                int h = use_heuristic ? abs(nr - er) + abs(nc - ec) : 0;   /* admissible: never overestimates */
                push((Node){g + h, g, nr, nc});
            }
        }
    }
    return -1;
}

int main(void)
{
    int e1, e2;
    int a = search(1, &e1);
    int d = search(0, &e2);
    printf("A*        : shortest path %d steps, %d nodes expanded\n", a, e1);
    printf("Dijkstra  : shortest path %d steps, %d nodes expanded\n", d, e2);
    printf("both agree: %s\n", a == d ? "yes" : "no");
    return 0;
}
