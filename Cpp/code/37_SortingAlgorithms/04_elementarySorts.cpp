// Bubble, selection and insertion sort as templates, with comparison and swap
// counts, adaptivity and stability shown.

#include <iostream>
#include <utility>
#include <vector>

struct Stats { long compares = 0, moves = 0; };

template <typename T>
Stats bubble_sort(std::vector<T> &a)
{
    Stats s;
    for (std::size_t pass = 0; pass + 1 < a.size(); ++pass)
    {
        bool swapped = false;
        for (std::size_t i = 0; i + 1 < a.size() - pass; ++i)
        {
            ++s.compares;
            if (a[i + 1] < a[i]) { std::swap(a[i], a[i + 1]); ++s.moves; swapped = true; }
        }
        if (!swapped) break;                              // already sorted: O(n) on sorted input
    }
    return s;
}

template <typename T>
Stats selection_sort(std::vector<T> &a)
{
    Stats s;
    for (std::size_t i = 0; i + 1 < a.size(); ++i)
    {
        std::size_t m = i;
        for (std::size_t j = i + 1; j < a.size(); ++j) { ++s.compares; if (a[j] < a[m]) m = j; }
        if (m != i) { std::swap(a[i], a[m]); ++s.moves; } // at most n - 1 swaps: good when writes are expensive
    }
    return s;
}

template <typename T>
Stats insertion_sort(std::vector<T> &a)
{
    Stats s;
    for (std::size_t i = 1; i < a.size(); ++i)
    {
        T v = std::move(a[i]);
        std::size_t j = i;
        while (j > 0 && (++s.compares, v < a[j - 1])) { a[j] = std::move(a[j - 1]); --j; ++s.moves; }
        a[j] = std::move(v);
    }
    return s;
}

int main()
{
    const std::vector<int> random{29, 3, 17, 8, 41, 5, 23, 11, 2, 37};
    std::vector<int> sorted{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto report = [](const char *name, Stats s) { std::cout << "  " << name << ": " << s.compares << " compares, " << s.moves << " moves\n"; };

    std::cout << "random input:\n";
    { auto v = random; report("bubble   ", bubble_sort(v)); }
    { auto v = random; report("selection", selection_sort(v)); }
    { auto v = random; report("insertion", insertion_sort(v)); }
    std::cout << "already sorted input:\n";
    { auto v = sorted; report("bubble   ", bubble_sort(v)); }
    { auto v = sorted; report("selection", selection_sort(v)); }
    { auto v = sorted; report("insertion", insertion_sort(v)); }

    struct P { int key; char tag; bool operator<(const P &o) const { return key < o.key; } };
    std::vector<P> items{{2, 'a'}, {1, 'b'}, {2, 'c'}, {1, 'd'}};
    auto st = items;
    insertion_sort(st);
    std::cout << "insertion is stable:";
    for (auto &p : st) std::cout << ' ' << p.key << p.tag;
    auto un = items;
    selection_sort(un);
    std::cout << "\nselection is not:   ";
    for (auto &p : un) std::cout << ' ' << p.key << p.tag;
    std::cout << '\n';
}
