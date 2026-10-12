// Last/median-of-three/random pivots on sorted input (O(n^2) vs O(n log n)),
// smaller-half recursion and three-way partitioning for duplicates.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

enum class Pivot { Last, MedianOfThree, Random };

std::mt19937 rng(42);                                    // fixed seed: repeatable
long comparisons;

int choose(std::vector<int> &a, int lo, int hi, Pivot p)
{
    switch (p)
    {
    case Pivot::Last: return hi;
    case Pivot::Random: return lo + static_cast<int>(rng() % static_cast<unsigned>(hi - lo + 1));
    case Pivot::MedianOfThree:
    {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] < a[lo]) std::swap(a[mid], a[lo]);
        if (a[hi] < a[lo]) std::swap(a[hi], a[lo]);
        if (a[hi] < a[mid]) std::swap(a[hi], a[mid]);
        return mid;                                      // the median of the three
    }
    }
    return hi;
}

void quick_sort(std::vector<int> &a, int lo, int hi, Pivot p)
{
    while (lo < hi)
    {
        std::swap(a[choose(a, lo, hi, p)], a[hi]);       // move the pivot to the end
        int pivot = a[hi], i = lo;
        for (int j = lo; j < hi; ++j)
        {
            ++comparisons;
            if (a[j] < pivot) std::swap(a[i++], a[j]);
        }
        std::swap(a[i], a[hi]);
        if (i - lo < hi - i) { quick_sort(a, lo, i - 1, p); lo = i + 1; }   // recurse into the smaller side,
        else                 { quick_sort(a, i + 1, hi, p); hi = i - 1; }   // loop on the larger: O(log n) stack
    }
}

// Three-way partition: elements equal to the pivot are grouped and never revisited.
void quick_sort_3way(std::vector<int> &a, int lo, int hi)
{
    if (lo >= hi) return;
    int pivot = a[lo + static_cast<int>(rng() % static_cast<unsigned>(hi - lo + 1))];
    int lt = lo, i = lo, gt = hi;
    while (i <= gt)
    {
        ++comparisons;
        if (a[i] < pivot) std::swap(a[lt++], a[i++]);
        else if (a[i] > pivot) std::swap(a[i], a[gt--]);
        else ++i;
    }
    quick_sort_3way(a, lo, lt - 1);
    quick_sort_3way(a, gt + 1, hi);
}

int main()
{
    const int n = 2000;
    std::vector<int> sorted(n);
    std::iota(sorted.begin(), sorted.end(), 0);          // already sorted: the classic worst case for a last-element pivot

    for (auto [name, p] : {std::pair{"last element ", Pivot::Last}, std::pair{"median of 3  ", Pivot::MedianOfThree},
                           std::pair{"random pivot  ", Pivot::Random}})
    {
        std::vector<int> a = sorted;
        comparisons = 0;
        quick_sort(a, 0, n - 1, p);
        std::cout << name << ": " << comparisons << " comparisons, sorted=" << std::boolalpha << std::is_sorted(a.begin(), a.end()) << '\n';
    }

    std::vector<int> few(n);                              // only 5 distinct values
    for (int i = 0; i < n; ++i) few[i] = i % 5;
    std::vector<int> b = few;
    comparisons = 0;
    quick_sort(b, 0, n - 1, Pivot::Random);
    std::cout << "2-way, 5 distinct values: " << comparisons << " comparisons\n";
    b = few;
    comparisons = 0;
    quick_sort_3way(b, 0, n - 1);
    std::cout << "3-way, 5 distinct values: " << comparisons << " comparisons, sorted=" << std::is_sorted(b.begin(), b.end()) << '\n';
}
