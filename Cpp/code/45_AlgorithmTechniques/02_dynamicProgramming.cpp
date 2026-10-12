// 0/1 knapsack (1-D table), LCS with reconstruction, edit distance (two rows),
// memoized climbing.

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int knapsack(const std::vector<int> &w, const std::vector<int> &v, int cap)
{
    std::vector<int> best(cap + 1, 0);                       // best[c] = best value with capacity c
    for (std::size_t i = 0; i < w.size(); ++i)
        for (int c = cap; c >= w[i]; --c)                    // downwards, so each item is used at most once
            best[c] = std::max(best[c], best[c - w[i]] + v[i]);
    return best[cap];
}

std::string lcs(const std::string &a, const std::string &b)
{
    std::vector<std::vector<int>> t(a.size() + 1, std::vector<int>(b.size() + 1, 0));
    for (std::size_t i = 1; i <= a.size(); ++i)
        for (std::size_t j = 1; j <= b.size(); ++j)
            t[i][j] = a[i - 1] == b[j - 1] ? t[i - 1][j - 1] + 1 : std::max(t[i - 1][j], t[i][j - 1]);
    std::string out;                                          // walk back through the table to rebuild one LCS
    for (std::size_t i = a.size(), j = b.size(); i > 0 && j > 0; )
    {
        if (a[i - 1] == b[j - 1]) { out += a[i - 1]; --i; --j; }
        else if (t[i - 1][j] >= t[i][j - 1]) --i;
        else --j;
    }
    return std::string(out.rbegin(), out.rend());
}

int edit_distance(const std::string &a, const std::string &b)
{
    std::vector<int> prev(b.size() + 1), cur(b.size() + 1);   // two rows are enough: O(min) memory
    for (std::size_t j = 0; j <= b.size(); ++j) prev[j] = static_cast<int>(j);
    for (std::size_t i = 1; i <= a.size(); ++i)
    {
        cur[0] = static_cast<int>(i);
        for (std::size_t j = 1; j <= b.size(); ++j)
            cur[j] = std::min({prev[j] + 1, cur[j - 1] + 1, prev[j - 1] + (a[i - 1] != b[j - 1])});
        std::swap(prev, cur);
    }
    return prev[b.size()];
}

// Top-down (memoized) version: count the ways to climb n stairs taking 1 or 2 steps.
unsigned long long ways(int n, std::unordered_map<int, unsigned long long> &memo)
{
    if (n <= 1) return 1;
    if (auto it = memo.find(n); it != memo.end()) return it->second;
    return memo[n] = ways(n - 1, memo) + ways(n - 2, memo);
}

int main()
{
    std::cout << "knapsack: " << knapsack({5, 8, 10, 3, 4, 7}, {50, 56, 60, 18, 20, 28}, 22) << '\n';
    std::cout << "LCS of AGGTAB and GXTXAYB: " << lcs("AGGTAB", "GXTXAYB") << '\n';
    std::cout << "edit distance kitten -> sitting: " << edit_distance("kitten", "sitting") << '\n';
    std::unordered_map<int, unsigned long long> memo;
    std::cout << "ways to climb 50 stairs: " << ways(50, memo) << '\n';
}
