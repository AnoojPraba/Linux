// Gray code: to and from binary, the reflected construction and the rotary-
// encoder use case; neighbours differ in one bit.

#include <bit>
#include <bitset>
#include <iostream>
#include <vector>

unsigned to_gray(unsigned n) { return n ^ (n >> 1); }

unsigned from_gray(unsigned g)
{
    unsigned n = 0;
    for (; g; g >>= 1) n ^= g;                                       // prefix xor
    return n;
}

std::vector<unsigned> gray_sequence(int bits)                        // reflected construction
{
    std::vector<unsigned> seq{0};
    for (int b = 0; b < bits; ++b)
        for (std::size_t i = seq.size(); i-- > 0;) seq.push_back(seq[i] | 1u << b);   // mirror the list, set a new top bit
    return seq;
}

int main()
{
    std::cout << "n  binary gray  bits changed\n";
    for (unsigned n = 0; n < 8; ++n)
        std::cout << n << "  " << std::bitset<3>(n) << "    " << std::bitset<3>(to_gray(n)) << "   "
                  << (n ? std::popcount(to_gray(n) ^ to_gray(n - 1)) : 0) << '\n';
    bool ok = true;
    for (unsigned n = 0; n < 5000; ++n) ok = ok && from_gray(to_gray(n)) == n;
    std::cout << "round trip for 0..4999: " << std::boolalpha << ok << '\n';
    std::cout << "reflected 3-bit sequence:";
    for (unsigned g : gray_sequence(3)) std::cout << ' ' << g;
    std::cout << "\nuses: rotary encoders (one bit changes between positions, no glitches), Karnaugh maps, hypercube traversal, solving Tower of Hanoi / puzzles\n";
}
