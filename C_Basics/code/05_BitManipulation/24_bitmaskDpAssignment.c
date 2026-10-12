// Bitmask DP: assign n tasks to n people with minimum cost, and count
// Hamiltonian-style orderings over subsets.

#include <stdio.h>

#define N 4

static const int cost[N][N] = {
    {9, 2, 7, 8},
    {6, 4, 3, 7},
    {5, 8, 1, 8},
    {7, 6, 9, 4}};

int main(void)
{
    int dp[1 << N];
    for (int m = 0; m < (1 << N); m++) dp[m] = 1 << 28;
    dp[0] = 0;                                          /* dp[mask] = cheapest way to give the tasks in mask to the first popcount(mask) people */
    for (int mask = 0; mask < (1 << N); mask++)
    {
        int person = __builtin_popcount(mask);          /* the next person to assign */
        if (person >= N || dp[mask] >= (1 << 28)) continue;
        for (int task = 0; task < N; task++)
            if (!(mask >> task & 1) && dp[mask] + cost[person][task] < dp[mask | 1 << task])
                dp[mask | 1 << task] = dp[mask] + cost[person][task];
    }
    printf("minimum total assignment cost = %d\n", dp[(1 << N) - 1]);

    /* number of permutations of 0..3 in which no element sits at its own index (derangements) via DP over masks */
    long ways[1 << N] = {0};
    ways[0] = 1;
    for (int mask = 0; mask < (1 << N); mask++)
    {
        int pos = __builtin_popcount(mask);
        for (int v = 0; v < N && pos < N; v++)
            if (!(mask >> v & 1) && v != pos) ways[mask | 1 << v] += ways[mask];
    }
    printf("derangements of 4 elements = %ld\n", ways[(1 << N) - 1]);
    return 0;
}
