// Maximum XOR with a binary trie: best pair, best partner for a query and the
// maximum xor subarray.

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

class XorTrie
{
    static constexpr int BITS = 8;
    std::vector<std::array<int, 2>> next_{{-1, -1}};
public:
    void insert(unsigned x)
    {
        int v = 0;
        for (int b = BITS - 1; b >= 0; --b)
        {
            int bit = x >> b & 1;
            if (next_[v][bit] < 0) { next_[v][bit] = static_cast<int>(next_.size()); next_.push_back({-1, -1}); }
            v = next_[v][bit];
        }
    }
    unsigned best_xor(unsigned x) const                               // max of x ^ y over inserted y
    {
        int v = 0;
        unsigned result = 0;
        for (int b = BITS - 1; b >= 0; --b)
        {
            int want = !(x >> b & 1);                                  // the opposite bit makes this bit of the xor a 1
            if (next_[v][want] >= 0) { result |= 1u << b; v = next_[v][want]; }
            else v = next_[v][!want];
        }
        return result;
    }
};

int main()
{
    std::vector<unsigned> a{3, 10, 5, 25, 2, 8};
    XorTrie t;
    unsigned best = 0;
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        if (i > 0) best = std::max(best, t.best_xor(a[i]));            // query before inserting: the trie must not be empty
        t.insert(a[i]);
    }
    std::cout << "maximum xor of two numbers: " << best << " (5 ^ 25)\n";

    XorTrie prefix;                                                    // subarray xor = prefix[j] ^ prefix[i]
    unsigned pre = 0, sub = 0;
    prefix.insert(0);
    for (unsigned x : a) { pre ^= x; sub = std::max(sub, prefix.best_xor(pre)); prefix.insert(pre); }
    std::cout << "maximum xor of a contiguous subarray: " << sub << '\n';
    std::cout << "O(n * bits) instead of O(n^2); bits = 8 here, use 31 or 63 for real input\n";
}
