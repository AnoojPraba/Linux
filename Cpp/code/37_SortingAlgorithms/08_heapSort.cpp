// Heap sort written by hand (sift-down) and with std::make_heap/sort_heap,
// plus partial_sort for the top k.

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

void sift_down(std::vector<int> &a, std::size_t n, std::size_t i)
{
    for (;;)
    {
        std::size_t l = 2 * i + 1, r = l + 1, largest = i;
        if (l < n && a[l] > a[largest]) largest = l;
        if (r < n && a[r] > a[largest]) largest = r;
        if (largest == i) return;
        std::swap(a[i], a[largest]);
        i = largest;
    }
}

void heap_sort(std::vector<int> &a)
{
    for (std::size_t i = a.size() / 2; i-- > 0;)          // build a max-heap in O(n)
        sift_down(a, a.size(), i);
    for (std::size_t end = a.size(); end-- > 1;)
    {
        std::swap(a[0], a[end]);                          // the maximum goes to its final place
        sift_down(a, end, 0);
    }
}

int main()
{
    std::vector<int> a{9, 4, 7, 1, 8, 2, 6, 3, 5, 0};
    auto b = a, c = a;
    heap_sort(a);
    std::cout << "hand-written heap sort :";
    for (int x : a) std::cout << ' ' << x;

    std::make_heap(b.begin(), b.end());                   // O(n)
    std::sort_heap(b.begin(), b.end());                   // repeated pop_heap: O(n log n)
    std::cout << "\nstd::make_heap + sort_heap:";
    for (int x : b) std::cout << ' ' << x;

    std::partial_sort(c.begin(), c.begin() + 3, c.end(), std::greater<>());   // only the 3 largest, in order: O(n log k)
    std::cout << "\npartial_sort, 3 largest :";
    for (int i = 0; i < 3; ++i) std::cout << ' ' << c[i];
    std::cout << "\nheap sort: O(n log n) always, O(1) extra space, not stable, poor cache locality\n";
}
