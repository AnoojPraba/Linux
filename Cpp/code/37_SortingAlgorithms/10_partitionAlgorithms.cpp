// Partitioning: std::partition, stable_partition, partition_point and the
// Dutch national flag with three groups.

#include <algorithm>
#include <iostream>
#include <vector>

void dutch_flag(std::vector<int> &a)                      // sort 0/1/2 in one pass, O(1) space
{
    std::size_t lo = 0, mid = 0, hi = a.size();
    while (mid < hi)
    {
        if (a[mid] == 0) std::swap(a[lo++], a[mid++]);
        else if (a[mid] == 1) ++mid;
        else std::swap(a[mid], a[--hi]);                  // the swapped-in element is unexamined: do not advance mid
    }
}

int main()
{
    std::vector<int> a{2, 0, 2, 1, 1, 0, 2, 0, 1};
    dutch_flag(a);
    std::cout << "dutch flag :";
    for (int x : a) std::cout << ' ' << x;

    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto even = [](int x) { return x % 2 == 0; };
    auto it = std::stable_partition(v.begin(), v.end(), even);   // keeps the relative order inside both groups
    std::cout << "\nstable_partition (evens first):";
    for (int x : v) std::cout << ' ' << x;
    std::cout << "\n  partition point is at index " << it - v.begin() << ", partition_point finds " << std::partition_point(v.begin(), v.end(), even) - v.begin();

    std::vector<int> u{9, 1, 8, 2, 7, 3, 6, 4, 5};
    auto cut = std::partition(u.begin(), u.end(), [](int x) { return x < 5; });   // fast but unordered inside each group
    std::cout << "\nstd::partition around 5: " << cut - u.begin() << " values below the cut, is_partitioned = " << std::boolalpha
              << std::is_partitioned(u.begin(), u.end(), [](int x) { return x < 5; }) << '\n';
}
