// Greedy scans: gas station circuit, jump game (reachability and minimum
// jumps) and best time to buy and sell stock.

#include <algorithm>
#include <iostream>
#include <vector>

int gas_start(const std::vector<int> &gas, const std::vector<int> &cost)
{
    int total = 0, tank = 0, start = 0;
    for (std::size_t i = 0; i < gas.size(); ++i)
    {
        total += gas[i] - cost[i];
        tank += gas[i] - cost[i];
        if (tank < 0) { start = static_cast<int>(i) + 1; tank = 0; }     // cannot pass i from any start before i
    }
    return total >= 0 ? start : -1;
}

bool can_reach(const std::vector<int> &a)
{
    std::size_t far = 0;
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        if (i > far) return false;
        far = std::max(far, i + a[i]);
    }
    return true;
}

int min_jumps(const std::vector<int> &a)
{
    int jumps = 0;
    std::size_t end = 0, far = 0;
    for (std::size_t i = 0; i + 1 < a.size(); ++i)
    {
        far = std::max(far, i + a[i]);
        if (i == end) { ++jumps; end = far; if (end + 1 >= a.size()) break; }   // forced to jump: take the best seen in this window
    }
    return jumps;
}

int max_profit(const std::vector<int> &prices)
{
    int best = 0, low = prices[0];
    for (int p : prices) { low = std::min(low, p); best = std::max(best, p - low); }   // best selling day for the cheapest buy so far
    return best;
}

int main()
{
    std::cout << "gas station start: " << gas_start({1, 2, 3, 4, 5}, {3, 4, 5, 1, 2}) << ", impossible: " << gas_start({2, 3, 4}, {3, 4, 3}) << '\n';
    std::cout << std::boolalpha << "jump {2,3,1,1,4}: " << can_reach({2, 3, 1, 1, 4}) << ", min jumps " << min_jumps({2, 3, 1, 1, 4}) << "; {3,2,1,0,4}: " << can_reach({3, 2, 1, 0, 4}) << '\n';
    std::cout << "best single trade in {7,1,5,3,6,4}: " << max_profit({7, 1, 5, 3, 6, 4}) << '\n';
    int unlimited = 0;
    std::vector<int> p{7, 1, 5, 3, 6, 4};
    for (std::size_t i = 1; i < p.size(); ++i) unlimited += std::max(0, p[i] - p[i - 1]);   // many trades: collect every upward step
    std::cout << "with unlimited trades: " << unlimited << '\n';
}
