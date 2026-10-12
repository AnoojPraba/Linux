// Top-down merge sort with a temporary buffer; stable, O(n log n) always.

#include <iostream>
#include <vector>

void merge_sort(std::vector<int> &a, std::vector<int> &tmp, int lo, int hi)   // sorts a[lo, hi)
{
    if (hi - lo < 2)
        return;
    int mid = lo + (hi - lo) / 2;
    merge_sort(a, tmp, lo, mid);
    merge_sort(a, tmp, mid, hi);

    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi)
        tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];    // <= keeps equal elements in order
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi)  tmp[k++] = a[j++];
    for (k = lo; k < hi; ++k)
        a[k] = tmp[k];
}

int main()
{
    std::vector<int> a{38, 27, 43, 3, 9, 82, 10};
    std::vector<int> tmp(a.size());
    merge_sort(a, tmp, 0, static_cast<int>(a.size()));
    for (int x : a)
        std::cout << x << ' ';
    std::cout << '\n';
}
