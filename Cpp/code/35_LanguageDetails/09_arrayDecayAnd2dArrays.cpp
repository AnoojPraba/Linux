// Array-to-pointer decay in parameters, reference-to-array template, 2-D
// arrays, std::array, vector<vector>.

#include <array>
#include <iostream>
#include <vector>

void print_flat(const int *a, int n)               // int a[] and int *a are the same parameter type
{
    std::cout << "inside the function sizeof(a) = " << sizeof(a) << " (a pointer), n = " << n << '\n';
}

template <std::size_t N>
int sum_ref(const int (&a)[N])                      // a reference to the array keeps its size
{
    int s = 0;
    for (int x : a)
        s += x;
    return s;
}

void print_matrix(const int m[][3], int rows)       // all dimensions except the first must be given
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < 3; ++j)
            std::cout << m[i][j] << ' ';
        std::cout << '\n';
    }
}

int main()
{
    int a[5] = {1, 2, 3, 4, 5};
    std::cout << "in main sizeof(a) = " << sizeof(a) << ", elements = " << sizeof(a) / sizeof(a[0]) << '\n';
    print_flat(a, 5);
    std::cout << "sum through a reference to the array: " << sum_ref(a) << '\n';

    int m[2][3] = {{1, 2, 3}, {4, 5, 6}};
    print_matrix(m, 2);
    std::cout << "m[1][2] = " << m[1][2] << ", rows = " << sizeof(m) / sizeof(m[0]) << ", columns = " << sizeof(m[0]) / sizeof(m[0][0]) << '\n';

    int transposed[3][2];
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            transposed[j][i] = m[i][j];
    std::cout << "transposed[2][1] = " << transposed[2][1] << '\n';

    std::array<std::array<int, 3>, 2> sa{{{1, 2, 3}, {4, 5, 6}}};              // knows its size and can be copied
    std::vector<std::vector<int>> vv(2, std::vector<int>(3, 0));                // sized at run time
    vv[1][2] = 7;
    std::cout << "std::array rows = " << sa.size() << ", vector<vector> value = " << vv[1][2] << '\n';
}
