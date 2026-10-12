// XOR and counting tricks for unique elements: single, two singles, single
// among triples, missing and duplicate numbers.

#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

int single(const std::vector<int> &a) { return std::accumulate(a.begin(), a.end(), 0, [](int x, int y) { return x ^ y; }); }   // pairs cancel

std::pair<int, int> two_singles(const std::vector<int> &a)
{
    int x = single(a), low = x & -x, first = 0, second = 0;           // a set bit of x tells the two singles apart
    for (int v : a) (v & low ? first : second) ^= v;
    return {first, second};
}

int single_among_triples(const std::vector<int> &a)
{
    int ones = 0, twos = 0;                                           // per-bit counters modulo 3
    for (int v : a) { ones = (ones ^ v) & ~twos; twos = (twos ^ v) & ~ones; }
    return ones;
}

std::pair<int, int> duplicate_and_missing(const std::vector<int> &a)  // values 1..n with one duplicated and one missing
{
    int xr = 0;
    for (std::size_t i = 0; i < a.size(); ++i) xr ^= a[i] ^ static_cast<int>(i + 1);   // duplicate ^ missing
    int bit = xr & -xr, g1 = 0, g2 = 0;
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        (a[i] & bit ? g1 : g2) ^= a[i];
        (static_cast<int>(i + 1) & bit ? g1 : g2) ^= static_cast<int>(i + 1);
    }
    for (int v : a) if (v == g1) return {g1, g2};
    return {g2, g1};
}

int missing_number(const std::vector<int> &a)                         // 0..n with one value missing
{
    int x = static_cast<int>(a.size());
    for (std::size_t i = 0; i < a.size(); ++i) x ^= static_cast<int>(i) ^ a[i];
    return x;
}

int main()
{
    std::cout << "single among pairs {4,1,2,1,2}: " << single({4, 1, 2, 1, 2}) << '\n';
    auto [p, q] = two_singles({1, 2, 1, 3, 2, 5});
    std::cout << "two singles in {1,2,1,3,2,5}: " << p << " and " << q << '\n';
    std::cout << "single among triples: " << single_among_triples({2, 2, 3, 2, 7, 7, 7, 9, 9, 9, 5, 5, 5}) << '\n';
    auto [dup, miss] = duplicate_and_missing({1, 2, 2, 4});
    std::cout << "duplicate " << dup << ", missing " << miss << " in {1,2,2,4}\n";
    std::cout << "missing number in 0..5 {0,1,3,4,5}: " << missing_number({0, 1, 3, 4, 5}) << '\n';
}
