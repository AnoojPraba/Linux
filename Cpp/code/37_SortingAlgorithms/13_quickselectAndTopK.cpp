// Selection without a full sort: hand-written quickselect, std::nth_element,
// std::partial_sort and a heap for the top k.

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

int quickselect(std::vector<int> a, std::size_t k)         // k-th smallest, 0-based, expected O(n)
{
    std::size_t lo = 0, hi = a.size() - 1;
    while (lo < hi)
    {
        std::swap(a[lo + (hi - lo) / 2], a[hi]);
        int pivot = a[hi];
        std::size_t store = lo;
        for (std::size_t i = lo; i < hi; ++i) if (a[i] < pivot) std::swap(a[i], a[store++]);
        std::swap(a[store], a[hi]);
        if (store == k) return a[k];
        if (store < k) lo = store + 1; else hi = store - 1;
    }
    return a[lo];
}

int main()
{
    std::vector<int> a{7, 10, 4, 3, 20, 15, 8, 1, 12};
    std::cout << "3rd smallest by quickselect: " << quickselect(a, 2) << '\n';

    auto b = a;
    std::nth_element(b.begin(), b.begin() + 4, b.end());   // the median lands at index 4; the sides are only partitioned
    std::cout << "median by nth_element      : " << b[4] << '\n';

    auto c = a;
    std::partial_sort(c.begin(), c.begin() + 3, c.end());  // the 3 smallest, sorted: O(n log k)
    std::cout << "3 smallest by partial_sort :" << ' ' << c[0] << ' ' << c[1] << ' ' << c[2] << '\n';

    std::priority_queue<int, std::vector<int>, std::greater<>> heap;   // min-heap of size k = the k largest of a stream
    for (int x : a) { heap.push(x); if (heap.size() > 3) heap.pop(); }
    std::cout << "3 largest by a size-3 heap :";
    while (!heap.empty()) { std::cout << ' ' << heap.top(); heap.pop(); }
    std::cout << "\nO(n) average vs O(n log n) for sorting everything; median-of-medians makes quickselect O(n) worst case\n";
}
