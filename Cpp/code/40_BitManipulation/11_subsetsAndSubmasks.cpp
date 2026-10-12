// Enumerating subsets with bitmasks: all subsets, all submasks of a mask,
// fixed-size subsets (Gosper's hack with <bit>) and subset-sum counting.

#include <bit>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::vector<std::string> items{"a", "b", "c", "d"};
    std::cout << "all subsets:";
    for (unsigned mask = 0; mask < (1u << items.size()); ++mask)
    {
        std::cout << " {";
        for (std::size_t i = 0; i < items.size(); ++i) if (mask >> i & 1) std::cout << items[i];
        std::cout << '}';
    }

    unsigned full = 0b1011;
    std::cout << "\nsubmasks of 1011 (decreasing):";
    for (unsigned sub = full;; sub = (sub - 1) & full)               // jumps to the next smaller submask
    {
        std::cout << ' ' << sub;
        if (sub == 0) break;
    }

    std::cout << "\nsubsets of size 2 (Gosper's hack):";
    for (unsigned x = 0b11; x < (1u << 4);)
    {
        std::cout << ' ' << x;
        unsigned c = x & -x, r = x + c;                              // next integer with the same popcount
        x = (((r ^ x) >> 2) / c) | r;
    }

    std::cout << "\nmasks with exactly 3 of 5 bits:";
    for (unsigned m = 0; m < 32; ++m) if (std::popcount(m) == 3) std::cout << ' ' << m;

    std::vector<int> w{3, 34, 4, 12, 5, 2};
    int ways = 0;
    for (unsigned mask = 0; mask < (1u << w.size()); ++mask)
    {
        int sum = 0;
        for (std::size_t i = 0; i < w.size(); ++i) if (mask >> i & 1) sum += w[i];
        ways += sum == 9;
    }
    std::cout << "\nsubsets of {3,34,4,12,5,2} with sum 9: " << ways << '\n';

    unsigned long long pairs = 0;                                    // sum over all masks of the number of submasks = 3^n
    for (unsigned m = 0; m < 16; ++m) for (unsigned s = m;; s = (s - 1) & m) { ++pairs; if (!s) break; }
    std::cout << "(mask, submask) pairs for n = 4: " << pairs << " = 3^4\n";
}
