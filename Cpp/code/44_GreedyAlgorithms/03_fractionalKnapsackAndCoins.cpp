// Greedy by ratio and by denomination: fractional knapsack (greedy is optimal)
// versus 0/1 knapsack and arbitrary coin systems (greedy fails).

#include <algorithm>
#include <iostream>
#include <vector>

struct Item { int weight, value; };

double fractional_knapsack(std::vector<Item> items, int capacity)
{
    std::sort(items.begin(), items.end(), [](const Item &a, const Item &b) { return a.value * b.weight > b.value * a.weight; });   // best ratio first
    double total = 0;
    for (const Item &it : items)
    {
        if (capacity >= it.weight) { capacity -= it.weight; total += it.value; }
        else { total += it.value * static_cast<double>(capacity) / it.weight; break; }                // take the fitting fraction of the last item
    }
    return total;
}

int greedy_coins(const std::vector<int> &coins_desc, int amount)
{
    int count = 0;
    for (int c : coins_desc) { count += amount / c; amount %= c; }
    return amount ? -1 : count;
}

int dp_coins(const std::vector<int> &coins, int amount)
{
    std::vector<int> best(amount + 1, 1 << 20);
    best[0] = 0;
    for (int x = 1; x <= amount; ++x) for (int c : coins) if (c <= x) best[x] = std::min(best[x], best[x - c] + 1);
    return best[amount];
}

int main()
{
    std::vector<Item> items{{10, 60}, {20, 100}, {30, 120}};
    std::cout << "fractional knapsack, capacity 50: " << fractional_knapsack(items, 50) << " (greedy is optimal)\n";

    auto sorted = items;
    std::sort(sorted.begin(), sorted.end(), [](const Item &a, const Item &b) { return a.value * b.weight > b.value * a.weight; });
    int cap = 50, greedy01 = 0;
    for (auto &it : sorted) if (it.weight <= cap) { cap -= it.weight; greedy01 += it.value; }
    std::vector<int> dp(51, 0);
    for (auto &it : items) for (int c = 50; c >= it.weight; --c) dp[c] = std::max(dp[c], dp[c - it.weight] + it.value);
    std::cout << "0/1 knapsack: greedy by ratio " << greedy01 << ", optimal " << dp[50] << " (greedy fails)\n";

    std::cout << "coins 25/10/5/1, amount 63: greedy " << greedy_coins({25, 10, 5, 1}, 63) << ", optimal " << dp_coins({25, 10, 5, 1}, 63) << '\n';
    std::cout << "coins 4/3/1, amount 6: greedy " << greedy_coins({4, 3, 1}, 6) << ", optimal " << dp_coins({4, 3, 1}, 6) << " (greedy fails)\n";
}
