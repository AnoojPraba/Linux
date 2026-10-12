// Race of sorting algorithms by counted comparisons on random, sorted and
// reversed input, using a counting comparator.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

struct Counted
{
    int v;
    static inline long compares = 0;
    bool operator<(const Counted &o) const { ++compares; return v < o.v; }
};

template <typename F>
long run(std::vector<Counted> data, F sorter)
{
    Counted::compares = 0;
    sorter(data);
    return Counted::compares;
}

int main()
{
    const std::size_t n = 2000;
    std::vector<Counted> random_(n), sorted_(n), reversed_(n);
    std::mt19937 rng(3);
    for (std::size_t i = 0; i < n; ++i) { random_[i].v = static_cast<int>(rng() % 100000); sorted_[i].v = static_cast<int>(i); reversed_[i].v = static_cast<int>(n - i); }

    auto insertion = [](std::vector<Counted> &a) {
        for (std::size_t i = 1; i < a.size(); ++i) { auto v = a[i]; std::size_t j = i; while (j > 0 && v < a[j - 1]) { a[j] = a[j - 1]; --j; } a[j] = v; }
    };
    auto std_sort = [](std::vector<Counted> &a) { std::sort(a.begin(), a.end()); };
    auto stable = [](std::vector<Counted> &a) { std::stable_sort(a.begin(), a.end()); };
    auto heap = [](std::vector<Counted> &a) { std::make_heap(a.begin(), a.end()); std::sort_heap(a.begin(), a.end()); };

    std::cout << "n = " << n << "              random   sorted  reversed\n";
    auto row = [&](const char *name, auto sorter) {
        std::cout << name << run(random_, sorter) << "  " << run(sorted_, sorter) << "  " << run(reversed_, sorter) << '\n';
    };
    std::cout.setf(std::ios::left);
    row("insertion sort   ", insertion);
    row("std::sort        ", std_sort);
    row("std::stable_sort ", stable);
    row("heap sort        ", heap);
    std::cout << "insertion is O(n) on sorted input and O(n^2) on reversed; the others stay near n log2 n = " << static_cast<long>(n * 10.97) << '\n';
}
