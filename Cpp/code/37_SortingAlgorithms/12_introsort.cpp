// Generic introsort: median-of-three quick sort with a depth limit, heap-sort
// fallback and insertion sort for small ranges.

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

template <typename It>
void introsort_impl(It first, It last, int depth)
{
    constexpr auto threshold = 16;
    while (last - first > threshold)
    {
        if (depth-- == 0)
        {
            std::make_heap(first, last);                  // too deep: heap sort keeps the worst case O(n log n)
            std::sort_heap(first, last);
            return;
        }
        It mid = first + (last - first) / 2;
        std::iter_swap(mid, std::prev(last));             // median of three, moved to the end as the pivot
        auto pivot = *std::prev(last);
        It cut = std::partition(first, std::prev(last), [&](const auto &x) { return x < pivot; });
        std::iter_swap(cut, std::prev(last));
        introsort_impl(cut + 1, last, depth);             // recurse into one side, loop on the other
        last = cut;
    }
    for (It i = first + 1; i < last; ++i)                  // insertion sort for small ranges
        std::rotate(std::upper_bound(first, i, *i), i, i + 1);
}

template <typename It>
void introsort(It first, It last)
{
    introsort_impl(first, last, 2 * static_cast<int>(std::log2(std::max<long>(1, last - first))));
}

int main()
{
    std::vector<int> v(5000);
    std::iota(v.begin(), v.end(), 0);
    std::sort(v.begin(), v.end(), [](int a, int b) { return (a * 7919) % 5003 < (b * 7919) % 5003; });   // a scrambled order
    auto w = v;
    introsort(v.begin(), v.end());
    std::cout << "sorted: " << std::boolalpha << std::is_sorted(v.begin(), v.end()) << ", first values " << v[0] << ' ' << v[1] << ' ' << v[2] << '\n';
    std::cout << "same as std::sort: " << (std::sort(w.begin(), w.end()), w == v) << '\n';

    std::vector<int> sorted_input(5000);
    std::iota(sorted_input.begin(), sorted_input.end(), 0);
    introsort(sorted_input.begin(), sorted_input.end());   // would be quadratic for a naive first-element pivot
    std::cout << "sorted input handled: " << std::is_sorted(sorted_input.begin(), sorted_input.end()) << '\n';
}
