// The binary search family by hand and in the STL: lower/upper bound,
// equal_range, first/last occurrence, counts and insert position.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

template <typename It, typename T>
It my_lower_bound(It first, It last, const T &key)         // first element >= key
{
    auto count = last - first;
    while (count > 0)
    {
        auto step = count / 2;
        It mid = first + step;
        if (*mid < key) { first = mid + 1; count -= step + 1; }
        else count = step;
    }
    return first;
}

int main()
{
    std::vector<int> a{1, 2, 4, 4, 4, 4, 7, 9, 12};
    auto lb = my_lower_bound(a.begin(), a.end(), 4);
    auto ub = std::upper_bound(a.begin(), a.end(), 4);     // first element > key
    std::cout << "4 occupies indexes " << lb - a.begin() << ".." << ub - a.begin() - 1 << " (count " << ub - lb << ")\n";

    auto [lo, hi] = std::equal_range(a.begin(), a.end(), 4);   // both bounds in one call
    std::cout << "equal_range gives the same range: " << std::boolalpha << (lo == lb && hi == ub) << '\n';
    std::cout << "binary_search 9: " << std::binary_search(a.begin(), a.end(), 9) << ", 5: " << std::binary_search(a.begin(), a.end(), 5) << '\n';
    std::cout << "insert position of 5 keeps the vector sorted: index " << std::lower_bound(a.begin(), a.end(), 5) - a.begin() << '\n';

    auto fl = std::upper_bound(a.begin(), a.end(), 8) - 1;     // floor(8): largest element <= 8
    auto ce = std::lower_bound(a.begin(), a.end(), 8);         // ceiling(8): smallest element >= 8
    std::cout << "floor(8) = " << *fl << ", ceiling(8) = " << *ce << '\n';

    auto first_big = std::partition_point(a.begin(), a.end(), [](int x) { return x < 7; });   // binary search on any monotone predicate
    std::cout << "first element >= 7 by partition_point: " << *first_big << '\n';

    std::vector<int> b{1, 2, 3};                               // the overflow trap in a hand-written midpoint
    int big_lo = 2'000'000'000, big_hi = 2'100'000'000;
    std::cout << "lo + (hi - lo) / 2 = " << big_lo + (big_hi - big_lo) / 2 << " (and std::midpoint gives " << std::midpoint(big_lo, big_hi) << ")\n";
}
