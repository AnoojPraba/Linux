// Searching matrices: staircase search for row/column-sorted matrices and one
// binary search for a fully sorted matrix.

#include <iostream>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

bool staircase(const Matrix &m, int key)                    // rows and columns sorted: O(R + C)
{
    int r = 0, c = static_cast<int>(m[0].size()) - 1;
    while (r < static_cast<int>(m.size()) && c >= 0)
    {
        if (m[r][c] == key) return true;
        if (m[r][c] > key) --c;                             // the rest of the column is larger: drop it
        else ++r;                                           // the rest of the row is smaller: drop it
    }
    return false;
}

bool flat_binary(const Matrix &m, int key)                  // each row continues the previous one: a single sorted sequence
{
    int cols = static_cast<int>(m[0].size()), lo = 0, hi = static_cast<int>(m.size()) * cols - 1;
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2, v = m[mid / cols][mid % cols];
        if (v == key) return true;
        if (v < key) lo = mid + 1; else hi = mid - 1;
    }
    return false;
}

int main()
{
    Matrix a{{1, 4, 7, 11, 15}, {2, 5, 8, 12, 19}, {3, 6, 9, 16, 22}, {10, 13, 14, 17, 24}};
    for (int key : {5, 20, 14}) std::cout << "staircase " << key << ": " << std::boolalpha << staircase(a, key) << '\n';
    Matrix b{{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};
    std::cout << "fully sorted matrix: 16 -> " << flat_binary(b, 16) << ", 13 -> " << flat_binary(b, 13) << '\n';
}
