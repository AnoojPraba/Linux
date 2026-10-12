// Interval greedy problems: merge intervals, insert interval, minimum arrows,
// minimum removals for non-overlap and covering a segment.

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using Iv = std::pair<int, int>;

std::vector<Iv> merge_intervals(std::vector<Iv> v)
{
    std::sort(v.begin(), v.end());
    std::vector<Iv> out;
    for (auto &iv : v)
        if (!out.empty() && iv.first <= out.back().second) out.back().second = std::max(out.back().second, iv.second);   // overlaps the last: extend it
        else out.push_back(iv);
    return out;
}

int min_arrows(std::vector<Iv> balloons)
{
    std::sort(balloons.begin(), balloons.end(), [](const Iv &a, const Iv &b) { return a.second < b.second; });
    int arrows = 0;
    long last = -1L << 40;
    for (auto &b : balloons) if (b.first > last) { ++arrows; last = b.second; }      // shoot at the earliest end
    return arrows;
}

int min_removals_for_no_overlap(std::vector<Iv> v)
{
    std::sort(v.begin(), v.end(), [](const Iv &a, const Iv &b) { return a.second < b.second; });
    int kept = 0, end = -(1 << 30);
    for (auto &iv : v) if (iv.first >= end) { ++kept; end = iv.second; }              // activity selection
    return static_cast<int>(v.size()) - kept;
}

int cover(std::vector<Iv> v, int target_start, int target_end)
{
    std::sort(v.begin(), v.end());
    int used = 0, reach = target_start;
    std::size_t i = 0;
    while (reach < target_end)
    {
        int best = reach;
        while (i < v.size() && v[i].first <= reach) best = std::max(best, v[i++].second);   // the farthest end among intervals starting inside
        if (best == reach) return -1;                                                      // a gap nothing covers
        reach = best;
        ++used;
    }
    return used;
}

int main()
{
    std::cout << "merged:";
    for (auto [a, b] : merge_intervals({{1, 3}, {8, 10}, {2, 6}, {15, 18}, {17, 20}})) std::cout << " [" << a << ',' << b << ']';
    std::cout << "\nminimum arrows for balloons: " << min_arrows({{10, 16}, {2, 8}, {1, 6}, {7, 12}}) << '\n';
    std::cout << "intervals to remove for no overlap: " << min_removals_for_no_overlap({{1, 2}, {2, 3}, {3, 4}, {1, 3}}) << '\n';
    std::cout << "fewest intervals covering [0, 12]: " << cover({{0, 3}, {2, 6}, {5, 9}, {1, 4}, {8, 12}, {3, 5}}, 0, 12) << '\n';
}
