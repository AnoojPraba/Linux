// N-Queens with bitmasks, permutations with next_permutation, subset sum with
// pruning.

#include <algorithm>
#include <bit>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

// N-Queens with bitmasks: columns and both diagonals are tracked as bit sets, so each test is O(1).
int queens(int n, std::uint32_t cols = 0, std::uint32_t d1 = 0, std::uint32_t d2 = 0, int row = 0)
{
    if (row == n) return 1;
    int count = 0;
    std::uint32_t free = ~(cols | d1 | d2) & ((1u << n) - 1);
    while (free)
    {
        std::uint32_t bit = free & -free;                    // the lowest free column
        free ^= bit;
        count += queens(n, cols | bit, (d1 | bit) << 1, (d2 | bit) >> 1, row + 1);
    }
    return count;
}

// Subsets of a (positive numbers) with the given sum; prune as soon as the sum is too large.
void subsets(const std::vector<int> &a, std::size_t i, int remaining, std::vector<int> &path, int &found)
{
    if (remaining == 0)
    {
        ++found;
        return;
    }
    if (i == a.size() || remaining < 0) return;               // dead end
    path.push_back(a[i]);
    subsets(a, i + 1, remaining - a[i], path, found);         // take a[i]
    path.pop_back();                                          // undo
    subsets(a, i + 1, remaining, path, found);                // skip a[i]
}

int main()
{
    for (int n : {4, 6, 8})
        std::cout << n << "-queens: " << queens(n) << " solutions\n";

    std::vector<int> v{1, 2, 3};
    std::cout << "permutations of 1 2 3:";
    do
    {
        std::cout << " [";
        for (int x : v) std::cout << x;
        std::cout << ']';
    } while (std::next_permutation(v.begin(), v.end()));     // lexicographic order, returns false after the last
    std::cout << '\n';

    std::vector<int> nums{3, 34, 4, 12, 5, 2, 8, 6}, path;
    int found = 0;
    subsets(nums, 0, 15, path, found);
    std::cout << "subsets of {3,34,4,12,5,2,8,6} with sum 15: " << found << '\n';
}
