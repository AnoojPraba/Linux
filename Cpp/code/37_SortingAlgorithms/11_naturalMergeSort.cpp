// Natural merge sort with std::inplace_merge: detect ascending runs and merge
// neighbours, the principle of Timsort.

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<std::size_t> find_runs(const std::vector<int> &a)   // starting index of every ascending run, plus the end
{
    std::vector<std::size_t> starts;
    for (std::size_t i = 0; i < a.size();)
    {
        starts.push_back(i);
        while (i + 1 < a.size() && a[i] <= a[i + 1]) ++i;
        ++i;
    }
    starts.push_back(a.size());
    return starts;
}

int main()
{
    std::vector<int> a{1, 4, 9, 2, 3, 8, 10, 5, 6, 7, 0};
    auto starts = find_runs(a);
    std::cout << "natural runs: " << starts.size() - 1 << '\n';

    int passes = 0;
    while (starts.size() > 2)
    {
        std::vector<std::size_t> next;
        for (std::size_t i = 0; i + 1 < starts.size(); i += 2)
        {
            next.push_back(starts[i]);
            if (i + 2 < starts.size())
                std::inplace_merge(a.begin() + starts[i], a.begin() + starts[i + 1], a.begin() + starts[i + 2]);   // stable merge
        }
        next.push_back(a.size());
        starts = next;
        ++passes;
    }
    std::cout << "sorted:";
    for (int x : a) std::cout << ' ' << x;
    std::cout << "\nmerge passes: " << passes << '\n';

    std::vector<int> sorted_input{1, 2, 3, 4, 5, 6};
    std::cout << "an already sorted input is a single run: " << find_runs(sorted_input).size() - 1 << " run, O(n)\n";
    std::cout << "Timsort adds: minimum run length (insertion sort), merge-order invariants on a stack of runs, and galloping merges\n";
}
