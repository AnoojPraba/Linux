// Travelling salesman by Held-Karp bitmask DP: O(2^n n^2) instead of O(n!),
// with tour reconstruction.

#include <stdio.h>

#define N 5
#define INF 1000000
static const int dist[N][N] = {
    {0, 10, 15, 20, 25},
    {10, 0, 35, 25, 30},
    {15, 35, 0, 30, 5},
    {20, 25, 30, 0, 12},
    {25, 30, 5, 12, 0}};

static int dp[1 << N][N], from[1 << N][N];

int main(void)
{
    for (int m = 0; m < (1 << N); m++)
        for (int i = 0; i < N; i++) dp[m][i] = INF;
    dp[1][0] = 0;                                      /* visited = {0}, standing at 0 */

    for (int mask = 1; mask < (1 << N); mask++)
        for (int last = 0; last < N; last++)
        {
            if (!(mask >> last & 1) || dp[mask][last] >= INF) continue;
            for (int next = 0; next < N; next++)
            {
                if (mask >> next & 1) continue;
                int nm = mask | 1 << next;
                if (dp[mask][last] + dist[last][next] < dp[nm][next])
                {
                    dp[nm][next] = dp[mask][last] + dist[last][next];
                    from[nm][next] = last;
                }
            }
        }

    int full = (1 << N) - 1, best = INF, last = 0;
    for (int i = 1; i < N; i++)
        if (dp[full][i] + dist[i][0] < best)
        {
            best = dp[full][i] + dist[i][0];
            last = i;
        }
    int tour[N], mask = full;
    for (int k = N - 1; k >= 1; k--)                   /* walk the predecessor table backwards */
    {
        tour[k] = last;
        int p = from[mask][last];
        mask ^= 1 << last;
        last = p;
    }
    tour[0] = 0;
    printf("shortest tour length = %d, tour:", best);
    for (int i = 0; i < N; i++) printf(" %d", tour[i]);
    printf(" 0\n");
    return 0;
}
