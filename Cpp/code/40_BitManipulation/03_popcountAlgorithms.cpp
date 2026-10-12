// Population count four ways: Kernighan, lookup table, SWAR parallel add and
// std::popcount, with Hamming distance and parity.

#include <array>
#include <bit>
#include <cstdint>
#include <iostream>

constexpr std::array<std::uint8_t, 256> make_table()
{
    std::array<std::uint8_t, 256> t{};
    for (int i = 1; i < 256; ++i) t[i] = static_cast<std::uint8_t>(t[i >> 1] + (i & 1));
    return t;
}
constexpr auto table = make_table();                                  // built at compile time

int kernighan(std::uint32_t x) { int c = 0; for (; x; x &= x - 1) ++c; return c; }                // cost = number of set bits
int by_table(std::uint32_t x) { return table[x & 255] + table[x >> 8 & 255] + table[x >> 16 & 255] + table[x >> 24]; }
int swar(std::uint32_t x)                                                                       // add neighbouring groups in parallel
{
    x = x - (x >> 1 & 0x55555555);
    x = (x & 0x33333333) + (x >> 2 & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    return static_cast<int>((x * 0x01010101) >> 24);
}

int main()
{
    std::uint32_t v = 0xF0F0A5C3;
    std::cout << "popcount of 0xF0F0A5C3: kernighan " << kernighan(v) << ", table " << by_table(v) << ", swar " << swar(v) << ", std::popcount " << std::popcount(v) << '\n';
    bool agree = true;
    for (std::uint32_t x = 0; x < 200000; ++x) agree = agree && kernighan(x) == swar(x) && swar(x) == by_table(x) && by_table(x) == std::popcount(x);
    std::cout << "all methods agree for 0..199999: " << std::boolalpha << agree << '\n';
    std::uint32_t a = 0b10110100, b = 0b10011110;
    std::cout << "Hamming distance 10110100 vs 10011110: " << std::popcount(a ^ b) << ", parity of v: " << (std::popcount(v) & 1) << '\n';
    std::cout << "std::popcount compiles to one instruction (popcnt / cnt) when the target supports it, else a SWAR sequence\n";
}
