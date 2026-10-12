// Non-comparison sorts: counting sort, LSD radix sort and bucket sort for
// integers and floating-point values.

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

std::vector<int> counting_sort(const std::vector<int> &a, int max_value)
{
    std::vector<int> count(max_value + 2, 0), out(a.size());
    for (int x : a) ++count[x + 1];
    for (std::size_t i = 1; i < count.size(); ++i) count[i] += count[i - 1];   // count[v] = first output index of value v
    for (int x : a) out[count[x]++] = x;                                        // iterating forward keeps it stable
    return out;
}

void radix_sort(std::vector<unsigned> &a)                 // LSD radix sort, 8 bits per pass
{
    std::vector<unsigned> tmp(a.size());
    for (unsigned shift = 0; shift < 32; shift += 8)
    {
        std::array<std::size_t, 257> count{};
        for (unsigned x : a) ++count[((x >> shift) & 255) + 1];
        for (std::size_t i = 1; i < 257; ++i) count[i] += count[i - 1];
        for (unsigned x : a) tmp[count[(x >> shift) & 255]++] = x;               // a stable pass per digit
        a.swap(tmp);
    }
}

void bucket_sort(std::vector<double> &a)                  // values in [0, 1)
{
    std::vector<std::vector<double>> bucket(a.size());
    for (double x : a) bucket[static_cast<std::size_t>(x * a.size())].push_back(x);
    a.clear();
    for (auto &b : bucket)
    {
        std::sort(b.begin(), b.end());
        a.insert(a.end(), b.begin(), b.end());
    }
}

int main()
{
    std::vector<int> a{4, 2, 2, 8, 3, 3, 1, 0, 7};
    auto s = counting_sort(a, 8);
    std::cout << "counting sort:";
    for (int x : s) std::cout << ' ' << x;

    std::vector<unsigned> r{170, 45, 75, 90, 802, 24, 2, 66, 4000000000u, 65536};
    radix_sort(r);
    std::cout << "\nradix sort   :";
    for (unsigned x : r) std::cout << ' ' << x;

    std::vector<double> d{0.78, 0.17, 0.39, 0.26, 0.72, 0.94, 0.21, 0.12, 0.23, 0.68};
    bucket_sort(d);
    std::cout << "\nbucket sort  :";
    for (double x : d) std::cout << ' ' << x;
    std::cout << "\ncounting: O(n + k); radix: O(d (n + k)); bucket: O(n) expected for uniform data - all beat n log n only for restricted keys\n";
}
