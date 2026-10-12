// Shell sort with several gap sequences (Shell, Knuth, Ciura) and a comparison
// count for each.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

long shell_sort(std::vector<int> a, const std::vector<std::size_t> &gaps)   // gaps in decreasing order, ending with 1
{
    long compares = 0;
    for (std::size_t gap : gaps)
        for (std::size_t i = gap; i < a.size(); ++i)
        {
            int v = a[i];
            std::size_t j = i;
            while (j >= gap && (++compares, a[j - gap] > v)) { a[j] = a[j - gap]; j -= gap; }   // gapped insertion sort
            a[j] = v;
        }
    return compares;
}

int main()
{
    std::vector<int> data(2000);
    std::iota(data.begin(), data.end(), 0);
    std::mt19937 rng(1);
    std::shuffle(data.begin(), data.end(), rng);

    std::vector<std::size_t> shell, knuth, ciura{1750, 701, 301, 132, 57, 23, 10, 4, 1};
    for (std::size_t g = data.size() / 2; g > 0; g /= 2) shell.push_back(g);                          // n/2, n/4, ..., 1
    std::vector<std::size_t> k;
    for (std::size_t g = 1; g < data.size() / 3; g = 3 * g + 1) k.push_back(g);                       // 1, 4, 13, 40, ...
    knuth.assign(k.rbegin(), k.rend());

    std::cout << "n = " << data.size() << " shuffled values\n";
    std::cout << "Shell gaps  (n/2, n/4, ..): " << shell_sort(data, shell) << " comparisons\n";
    std::cout << "Knuth gaps  (3h+1)        : " << shell_sort(data, knuth) << " comparisons\n";
    std::cout << "Ciura gaps  (empirical)   : " << shell_sort(data, ciura) << " comparisons\n";
    std::cout << "plain insertion sort would need about " << data.size() * data.size() / 4 << "\n";
}
