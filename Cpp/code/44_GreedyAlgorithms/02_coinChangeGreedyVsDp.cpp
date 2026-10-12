// Coins 4/3/1 for amount 6: greedy gives 3 coins, DP gives 2.

#include <algorithm>
#include <iostream>
#include <vector>

int greedy(std::vector<int> coins, int amount)
{
    std::sort(coins.rbegin(), coins.rend());               // largest first
    int count = 0;
    for (int c : coins)
        while (amount >= c)
        {
            amount -= c;
            ++count;
        }
    return amount == 0 ? count : -1;
}

int dp(const std::vector<int> &coins, int amount)
{
    const int INF = 1000000;
    std::vector<int> best(amount + 1, INF);                // best[x] = fewest coins that make x
    best[0] = 0;
    for (int x = 1; x <= amount; ++x)
        for (int c : coins)
            if (c <= x)
                best[x] = std::min(best[x], best[x - c] + 1);
    return best[amount] >= INF ? -1 : best[amount];
}

int main()
{
    std::vector<int> normal{25, 10, 5, 1}, odd{4, 3, 1};
    std::cout << "amount 63, coins 25/10/5/1 : greedy=" << greedy(normal, 63) << " dp=" << dp(normal, 63) << '\n';
    std::cout << "amount 6,  coins 4/3/1     : greedy=" << greedy(odd, 6) << " dp=" << dp(odd, 6) << '\n';
}
