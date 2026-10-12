// External sorting simulated in memory: sort chunk-sized runs, then k-way
// merge them with a priority_queue.

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

int main()
{
    std::vector<int> data{42, 7, 19, 3, 88, 15, 61, 2, 34, 9, 50, 27, 11, 70};
    const std::size_t memory = 4;                          // only this many numbers fit in "memory" at once

    std::vector<std::vector<int>> runs;
    for (std::size_t i = 0; i < data.size(); i += memory)
    {
        std::vector<int> run(data.begin() + i, data.begin() + std::min(data.size(), i + memory));
        std::sort(run.begin(), run.end());                 // phase 1: each run is sorted in memory and written out
        runs.push_back(run);
    }
    for (std::size_t r = 0; r < runs.size(); ++r)
    {
        std::cout << "run " << r << ":";
        for (int x : runs[r]) std::cout << ' ' << x;
        std::cout << '\n';
    }

    using Head = std::tuple<int, std::size_t, std::size_t>;   // (value, run, index in run)
    std::priority_queue<Head, std::vector<Head>, std::greater<>> heap;
    for (std::size_t r = 0; r < runs.size(); ++r) heap.emplace(runs[r][0], r, 0);

    std::cout << "merged:";
    while (!heap.empty())                                  // phase 2: only one element per run is in memory
    {
        auto [value, r, i] = heap.top();
        heap.pop();
        std::cout << ' ' << value;
        if (i + 1 < runs[r].size()) heap.emplace(runs[r][i + 1], r, i + 1);
    }
    std::cout << "\nmemory in phase 2: " << runs.size() << " heads, independent of the data size; passes = log_k(runs)\n";
}
