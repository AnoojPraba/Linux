// Classic bitwise interview problems: counting bits for 0..n, sum of two
// integers, maximum product of word lengths, xor queries and bit-flip counts.

#include <bit>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> count_bits_upto(int n)                               // dp: bits[i] = bits[i >> 1] + (i & 1)
{
    std::vector<int> bits(n + 1, 0);
    for (int i = 1; i <= n; ++i) bits[i] = bits[i >> 1] + (i & 1);
    return bits;
}

int max_product_of_disjoint_words(const std::vector<std::string> &words)
{
    std::vector<unsigned> mask(words.size(), 0);                      // one bit per letter: disjoint words have mask a & mask b == 0
    for (std::size_t i = 0; i < words.size(); ++i) for (char c : words[i]) mask[i] |= 1u << (c - 'a');
    int best = 0;
    for (std::size_t i = 0; i < words.size(); ++i)
        for (std::size_t j = i + 1; j < words.size(); ++j)
            if (!(mask[i] & mask[j])) best = std::max(best, static_cast<int>(words[i].size() * words[j].size()));
    return best;
}

int bit_flips_to_convert(int a, int b) { return std::popcount(static_cast<unsigned>(a ^ b)); }

int main()
{
    std::cout << "set bits for 0..8:";
    for (int c : count_bits_upto(8)) std::cout << ' ' << c;

    std::cout << "\nmax product of two words with no common letter: " << max_product_of_disjoint_words({"abcw", "baz", "foo", "bar", "xtfn", "abcdef"}) << " (abcw * xtfn)";

    std::vector<int> a{1, 3, 4, 8};                                   // range xor queries with a prefix xor array
    std::vector<int> pre(a.size() + 1, 0);
    for (std::size_t i = 0; i < a.size(); ++i) pre[i + 1] = pre[i] ^ a[i];
    std::cout << "\nxor of a[1..3] = " << (pre[4] ^ pre[1]) << " (3 ^ 4 ^ 8)";

    std::cout << "\nbit flips to turn 29 (11101) into 15 (01111): " << bit_flips_to_convert(29, 15);
    unsigned n = 0b1011'0000;
    std::cout << "\nlowest set bit index of 10110000: " << std::countr_zero(n) << ", highest: " << std::bit_width(n) - 1 << ", clear lowest: " << (n & (n - 1)) << '\n';
    std::cout << "ones-complement of 5 in 3 bits: " << (~5u & 7u) << ", binary gap of 1000100001: ";
    unsigned v = 0b1000100001, best = 0, run = 0;
    bool seen_one = false;
    for (; v; v >>= 1)
    {
        if (v & 1) { best = std::max(best, run); run = 0; seen_one = true; }
        else if (seen_one) ++run;
    }
    std::cout << best << '\n';
}
