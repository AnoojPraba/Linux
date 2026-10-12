// Bitmask DP: minimum-cost assignment, derangements by masks and the sum-over-
// subsets (SOS) DP that sums a function over all submasks in O(n 2^n).

#include <algorithm>
#include <bit>
#include <iostream>
#include <vector>

int main()
{
    const int N = 4;
    int cost[N][N] = {{9, 2, 7, 8}, {6, 4, 3, 7}, {5, 8, 1, 8}, {7, 6, 9, 4}};
    std::vector<int> dp(1 << N, 1 << 28);
    dp[0] = 0;                                                        // dp[mask] = cheapest way to give the tasks in mask to the first popcount(mask) people
    for (int mask = 0; mask < (1 << N); ++mask)
    {
        int person = std::popcount(static_cast<unsigned>(mask));
        if (person >= N || dp[mask] >= (1 << 28)) continue;
        for (int task = 0; task < N; ++task)
            if (!(mask >> task & 1)) dp[mask | 1 << task] = std::min(dp[mask | 1 << task], dp[mask] + cost[person][task]);
    }
    std::cout << "minimum assignment cost: " << dp.back() << '\n';

    std::vector<long> ways(1 << N, 0);
    ways[0] = 1;
    for (int mask = 0; mask < (1 << N); ++mask)
    {
        int pos = std::popcount(static_cast<unsigned>(mask));
        for (int v = 0; v < N && pos < N; ++v) if (!(mask >> v & 1) && v != pos) ways[mask | 1 << v] += ways[mask];
    }
    std::cout << "derangements of 4: " << ways.back() << '\n';

    std::vector<long> f{1, 2, 3, 4, 5, 6, 7, 8};                      // f[mask] for 3 bits
    auto sos = f;
    for (int bit = 0; bit < 3; ++bit)                                 // after processing bit b, sos[m] sums over submasks differing only in bits <= b
        for (int mask = 0; mask < 8; ++mask) if (mask >> bit & 1) sos[mask] += sos[mask ^ 1 << bit];
    std::cout << "sum over subsets:";
    for (long v : sos) std::cout << ' ' << v;
    long brute = 0;
    for (int sub = 7;; sub = (sub - 1) & 7) { brute += f[sub]; if (!sub) break; }
    std::cout << "\nsos[7] = " << sos[7] << " equals the brute-force sum over all 8 submasks: " << brute << '\n';
}
