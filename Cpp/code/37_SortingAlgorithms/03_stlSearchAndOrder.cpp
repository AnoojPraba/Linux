// sort, stable_sort, binary_search, lower_bound/upper_bound, nth_element.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::vector<int> v{40, 10, 30, 20, 50, 20, 60};
    std::sort(v.begin(), v.end());
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "binary_search 30: " << std::boolalpha << std::binary_search(v.begin(), v.end(), 30) << '\n';
    auto lo = std::lower_bound(v.begin(), v.end(), 20);       // first element >= 20
    auto hi = std::upper_bound(v.begin(), v.end(), 20);       // first element > 20
    std::cout << "20 appears " << (hi - lo) << " times, first at index " << (lo - v.begin()) << '\n';

    std::vector<int> w{9, 1, 8, 2, 7, 3, 6, 4, 5};
    std::nth_element(w.begin(), w.begin() + 4, w.end());       // the 5th smallest lands at index 4
    std::cout << "5th smallest = " << w[4] << '\n';

    struct Rec { std::string name; int grade; };
    std::vector<Rec> r{{"ann", 2}, {"raj", 1}, {"li", 2}, {"sam", 1}};
    std::stable_sort(r.begin(), r.end(), [](const Rec &a, const Rec &b) { return a.grade < b.grade; });
    for (const auto &x : r) std::cout << x.name << ':' << x.grade << ' ';
    std::cout << '\n';
}
