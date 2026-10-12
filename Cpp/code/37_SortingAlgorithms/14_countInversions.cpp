// Counting inversions with merge sort (O(n log n)) and with a Fenwick tree
// over compressed values.

#include <algorithm>
#include <iostream>
#include <vector>

long merge_count(std::vector<int> &a, std::vector<int> &tmp, std::size_t lo, std::size_t hi)
{
    if (hi - lo < 2) return 0;
    std::size_t mid = lo + (hi - lo) / 2;
    long inv = merge_count(a, tmp, lo, mid) + merge_count(a, tmp, mid, hi);
    std::size_t i = lo, j = mid, k = lo;
    while (i < mid && j < hi)
    {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else { tmp[k++] = a[j++]; inv += static_cast<long>(mid - i); }   // a[j] jumps over every remaining left element
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    std::copy(tmp.begin() + lo, tmp.begin() + hi, a.begin() + lo);
    return inv;
}

long fenwick_count(const std::vector<int> &a)
{
    std::vector<int> sorted = a;
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());       // coordinate compression
    std::vector<int> bit(sorted.size() + 1, 0);
    long inv = 0;
    for (std::size_t idx = a.size(); idx-- > 0;)             // right to left: count smaller elements already seen
    {
        std::size_t r = std::lower_bound(sorted.begin(), sorted.end(), a[idx]) - sorted.begin();   // rank, 0-based
        for (std::size_t i = r; i > 0; i -= i & -i) inv += bit[i];                                   // prefix sum of ranks < r
        for (std::size_t i = r + 1; i < bit.size(); i += i & -i) ++bit[i];
    }
    return inv;
}

int main()
{
    std::vector<int> a{8, 4, 2, 1, 7, 3}, b = a, tmp(a.size());
    std::cout << "merge sort inversions   : " << merge_count(b, tmp, 0, b.size()) << '\n';
    std::cout << "Fenwick tree inversions : " << fenwick_count(a) << '\n';
    std::vector<int> rev{5, 4, 3, 2, 1};
    std::cout << "reversed 5 elements     : " << fenwick_count(rev) << " (= 5 * 4 / 2)\n";
}
