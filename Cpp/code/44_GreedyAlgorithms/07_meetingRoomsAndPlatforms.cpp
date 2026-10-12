// Minimum meeting rooms / railway platforms: sort start and end times and
// sweep, or use a min-heap of end times.

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

int platforms(std::vector<int> arr, std::vector<int> dep)
{
    std::sort(arr.begin(), arr.end());
    std::sort(dep.begin(), dep.end());                                  // sorted independently: only the counts matter
    int cur = 0, best = 0;
    for (std::size_t i = 0, j = 0; i < arr.size();)
    {
        if (arr[i] <= dep[j]) { ++cur; ++i; best = std::max(best, cur); }
        else { --cur; ++j; }
    }
    return best;
}

int rooms_with_heap(std::vector<std::pair<int, int>> meetings)         // (start, end)
{
    std::sort(meetings.begin(), meetings.end());
    std::priority_queue<int, std::vector<int>, std::greater<>> ends;    // end times of rooms currently in use
    for (auto [s, e] : meetings)
    {
        if (!ends.empty() && ends.top() <= s) ends.pop();               // the earliest-finishing room is free: reuse it
        ends.push(e);
    }
    return static_cast<int>(ends.size());
}

int main()
{
    std::cout << "minimum platforms: " << platforms({900, 940, 950, 1100, 1500, 1800}, {910, 1200, 1120, 1130, 1900, 2000}) << '\n';
    std::cout << "rooms for (0,30) (5,10) (15,20): " << rooms_with_heap({{0, 30}, {5, 10}, {15, 20}}) << ", for (7,10) (2,4): " << rooms_with_heap({{7, 10}, {2, 4}}) << '\n';
}
