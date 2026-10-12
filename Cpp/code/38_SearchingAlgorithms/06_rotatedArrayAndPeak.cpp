// Binary search without an exact target: rotated sorted arrays (min and
// search), peak element and the single element in sorted pairs.

#include <iostream>
#include <vector>

int find_min(const std::vector<int> &a)
{
    int lo = 0, hi = static_cast<int>(a.size()) - 1;
    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] > a[hi]) lo = mid + 1;
        else if (a[mid] < a[hi]) hi = mid;
        else --hi;                                          // duplicates: cannot tell which side, shrink by one
    }
    return lo;
}

int search_rotated(const std::vector<int> &a, int key)
{
    int lo = 0, hi = static_cast<int>(a.size()) - 1;
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == key) return mid;
        if (a[lo] <= a[mid]) { if (a[lo] <= key && key < a[mid]) hi = mid - 1; else lo = mid + 1; }   // left half sorted
        else                 { if (a[mid] < key && key <= a[hi]) lo = mid + 1; else hi = mid - 1; }   // right half sorted
    }
    return -1;
}

int peak(const std::vector<int> &a)
{
    int lo = 0, hi = static_cast<int>(a.size()) - 1;
    while (lo < hi) { int mid = lo + (hi - lo) / 2; if (a[mid] < a[mid + 1]) lo = mid + 1; else hi = mid; }
    return lo;
}

int single_in_pairs(const std::vector<int> &a)
{
    int lo = 0, hi = static_cast<int>(a.size()) - 1;
    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (mid % 2) --mid;
        if (a[mid] == a[mid + 1]) lo = mid + 2; else hi = mid;
    }
    return a[lo];
}

int main()
{
    std::vector<int> r{15, 18, 2, 3, 6, 12};
    std::cout << "rotation point " << find_min(r) << ", search 3 -> " << search_rotated(r, 3) << ", 15 -> " << search_rotated(r, 15) << ", 7 -> " << search_rotated(r, 7) << '\n';
    std::cout << "min with duplicates {2,5,6,0,0,1,2}: index " << find_min({2, 5, 6, 0, 0, 1, 2}) << '\n';
    std::cout << "peak of {1,3,8,12,9,4,2}: index " << peak({1, 3, 8, 12, 9, 4, 2}) << '\n';
    std::cout << "single element in {1,1,2,3,3,4,4,8,8}: " << single_in_pairs({1, 1, 2, 3, 3, 4, 4, 8, 8}) << '\n';
}
