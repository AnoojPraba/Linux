// Travelling salesman by Held-Karp bitmask DP (O(2^n n^2)) with tour
// reconstruction, compared with brute force over permutations.

#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

int main()
{
    const int n = 5;
    int dist[n][n] = {{0, 10, 15, 20, 25}, {10, 0, 35, 25, 30}, {15, 35, 0, 30, 5}, {20, 25, 30, 0, 12}, {25, 30, 5, 12, 0}};
    const int INF = std::numeric_limits<int>::max() / 2;

    std::vector<std::vector<int>> dp(1 << n, std::vector<int>(n, INF)), from(1 << n, std::vector<int>(n, -1));
    dp[1][0] = 0;                                                         // visited = {0}, standing at 0
    for (int mask = 1; mask < (1 << n); ++mask)
        for (int last = 0; last < n; ++last)
        {
            if (!(mask >> last & 1) || dp[mask][last] >= INF) continue;
            for (int next = 0; next < n; ++next)
            {
                if (mask >> next & 1) continue;
                int nm = mask | 1 << next;
                if (dp[mask][last] + dist[last][next] < dp[nm][next]) { dp[nm][next] = dp[mask][last] + dist[last][next]; from[nm][next] = last; }
            }
        }
    int full = (1 << n) - 1, best = INF, last = 0;
    for (int i = 1; i < n; ++i) if (dp[full][i] + dist[i][0] < best) { best = dp[full][i] + dist[i][0]; last = i; }
    std::vector<int> tour;
    for (int mask = full, v = last; v >= 0 && mask; )
    {
        tour.push_back(v);
        int p = from[mask][v];
        mask ^= 1 << v;
        v = p;
    }
    std::reverse(tour.begin(), tour.end());
    std::cout << "Held-Karp: shortest tour length " << best << ", tour:";
    for (int v : tour) std::cout << ' ' << v;
    std::cout << " 0\n";

    std::vector<int> perm(n - 1);                                         // brute force: fix city 0, try all (n-1)! orders
    std::iota(perm.begin(), perm.end(), 1);
    int brute = INF;
    do
    {
        int len = dist[0][perm[0]];
        for (int i = 0; i + 1 < n - 1; ++i) len += dist[perm[i]][perm[i + 1]];
        brute = std::min(brute, len + dist[perm.back()][0]);
    } while (std::next_permutation(perm.begin(), perm.end()));
    std::cout << "brute force agrees: " << std::boolalpha << (brute == best) << " (" << n - 1 << "! = 24 orders)\n";
    std::cout << "Held-Karp O(2^n n^2) vs O(n!): fine up to n ~ 20; TSP is NP-hard\n";
}
