// 0/1 knapsack with a fractional upper bound; prints how many nodes of the
// full tree were visited.

#include <stdio.h>

#define N 6

/* sorted by value per weight, highest first: 10, 7, 6, 6, 5, 4 */
static const int weight[N] = {5, 8, 10, 3, 4, 7};
static const int value[N]  = {50, 56, 60, 18, 20, 28};
static const int cap = 22;

static int best;
static long nodes;

/* Upper bound: take the remaining items by value/weight and allow a fraction of the last one. */
static double bound(int i, int w, int v)
{
    double b = v;
    for (; i < N && w + weight[i] <= cap; i++)
    {
        w += weight[i];
        b += value[i];
    }
    if (i < N)
        b += (double)(cap - w) * value[i] / weight[i];
    return b;
}

static void search(int i, int w, int v)
{
    nodes++;
    if (w > cap) return;                        /* infeasible branch */
    if (v > best) best = v;
    if (i == N) return;
    if (bound(i, w, v) <= best) return;         /* even the optimistic estimate cannot win: prune */
    search(i + 1, w + weight[i], v + value[i]); /* branch: take item i */
    search(i + 1, w, v);                        /* branch: skip item i */
}

int main(void)
{
    search(0, 0, 0);
    printf("best value  : %d\n", best);
    printf("nodes visited: %ld of %d possible\n", nodes, (1 << (N + 1)) - 1);
    return 0;
}
