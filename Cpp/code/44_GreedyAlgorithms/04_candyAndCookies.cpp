// Greedy distribution problems: minimum candies for rating neighbours (two
// passes), assigning cookies and boats carrying two people.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int candies(const std::vector<int> &rating)
{
    std::vector<int> give(rating.size(), 1);
    for (std::size_t i = 1; i < rating.size(); ++i) if (rating[i] > rating[i - 1]) give[i] = give[i - 1] + 1;                 // left neighbours
    for (std::size_t i = rating.size() - 1; i-- > 0;) if (rating[i] > rating[i + 1]) give[i] = std::max(give[i], give[i + 1] + 1);   // right neighbours
    return std::accumulate(give.begin(), give.end(), 0);
}

int assign_cookies(std::vector<int> greed, std::vector<int> size)
{
    std::sort(greed.begin(), greed.end());
    std::sort(size.begin(), size.end());
    std::size_t child = 0;
    for (int cookie : size) if (child < greed.size() && cookie >= greed[child]) ++child;   // the smallest cookie that satisfies the least greedy child
    return static_cast<int>(child);
}

int boats(std::vector<int> weights, int limit)                          // each boat carries at most two people
{
    std::sort(weights.begin(), weights.end());
    int count = 0;
    for (long lo = 0, hi = static_cast<long>(weights.size()) - 1; lo <= hi; --hi, ++count)
        if (weights[lo] + weights[hi] <= limit) ++lo;                    // the lightest rides with the heaviest if they fit
    return count;
}

int main()
{
    std::cout << "candies for 1 0 2: " << candies({1, 0, 2}) << ", 1 2 2: " << candies({1, 2, 2}) << ", 1 3 4 5 2: " << candies({1, 3, 4, 5, 2}) << '\n';
    std::cout << "cookies: greed 1 2 3 / sizes 1 1 -> " << assign_cookies({1, 2, 3}, {1, 1}) << ", greed 1 2 / sizes 1 2 3 -> " << assign_cookies({1, 2}, {1, 2, 3}) << '\n';
    std::cout << "boats for weights 3 2 2 1 with limit 3: " << boats({3, 2, 2, 1}, 3) << ", weights 3 5 3 4 limit 5: " << boats({3, 5, 3, 4}, 5) << '\n';
}
