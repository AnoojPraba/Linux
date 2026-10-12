// Cycle sort (minimum number of writes) and pancake sort (only prefix
// reversals).

#include <algorithm>
#include <iostream>
#include <vector>

int cycle_sort(std::vector<int> &a)                       // returns the number of writes
{
    int writes = 0;
    for (std::size_t start = 0; start + 1 < a.size(); ++start)
    {
        int item = a[start];
        std::size_t pos = start;
        for (std::size_t i = start + 1; i < a.size(); ++i) if (a[i] < item) ++pos;   // final position = count of smaller items
        if (pos == start) continue;
        while (item == a[pos]) ++pos;                     // skip equal elements
        std::swap(item, a[pos]); ++writes;
        while (pos != start)                              // rotate the rest of the cycle
        {
            pos = start;
            for (std::size_t i = start + 1; i < a.size(); ++i) if (a[i] < item) ++pos;
            while (item == a[pos]) ++pos;
            std::swap(item, a[pos]); ++writes;
        }
    }
    return writes;
}

std::vector<int> pancake_sort(std::vector<int> &a)        // returns the flip sizes
{
    std::vector<int> flips;
    for (std::size_t size = a.size(); size > 1; --size)
    {
        std::size_t m = std::max_element(a.begin(), a.begin() + size) - a.begin();
        if (m == size - 1) continue;
        if (m > 0) { std::reverse(a.begin(), a.begin() + m + 1); flips.push_back(static_cast<int>(m + 1)); }
        std::reverse(a.begin(), a.begin() + size);        // flip the maximum into its final place
        flips.push_back(static_cast<int>(size));
    }
    return flips;
}

int main()
{
    std::vector<int> a{20, 40, 50, 10, 30, 10, 50};
    int writes = cycle_sort(a);
    std::cout << "cycle sort:";
    for (int x : a) std::cout << ' ' << x;
    std::cout << " (" << writes << " writes, at most n - 1 = " << a.size() - 1 << " for a permutation)\n";

    std::vector<int> p{3, 6, 1, 9, 4, 8, 2};
    auto flips = pancake_sort(p);
    std::cout << "pancake sort:";
    for (int x : p) std::cout << ' ' << x;
    std::cout << " using flips:";
    for (int f : flips) std::cout << ' ' << f;
    std::cout << " (at most 2n - 3 = " << 2 * p.size() - 3 << ")\n";
}
