// Reversing and rotating bits: reverse a word (loop, lookup, divide and
// conquer), swap nibbles/bytes, rotate and parity.

#include <bit>
#include <cstdint>
#include <iostream>

std::uint32_t reverse_loop(std::uint32_t x)
{
    std::uint32_t r = 0;
    for (int i = 0; i < 32; ++i) { r = r << 1 | (x & 1); x >>= 1; }
    return r;
}

std::uint32_t reverse_swap(std::uint32_t x)                           // swap halves at every scale: 5 steps instead of 32
{
    x = (x >> 1 & 0x55555555) | (x & 0x55555555) << 1;
    x = (x >> 2 & 0x33333333) | (x & 0x33333333) << 2;
    x = (x >> 4 & 0x0F0F0F0F) | (x & 0x0F0F0F0F) << 4;
    x = (x >> 8 & 0x00FF00FF) | (x & 0x00FF00FF) << 8;
    return x >> 16 | x << 16;
}

std::uint32_t byte_swap(std::uint32_t x)                              // endianness conversion (htonl)
{
    return (x & 0xFF) << 24 | (x & 0xFF00) << 8 | (x >> 8 & 0xFF00) | x >> 24;
}

int main()
{
    std::uint32_t x = 0x12345678;
    std::cout << std::hex << "reverse (loop) : " << reverse_loop(x) << "\nreverse (swap) : " << reverse_swap(x) << "\nbyte swap      : " << byte_swap(x) << '\n';
    std::cout << "rotate left 8  : " << std::rotl(x, 8) << ", right 4: " << std::rotr(x, 4) << '\n';
    std::cout << "swap nibbles of 0xAB: " << ((0xABu & 0x0F) << 4 | (0xABu >> 4)) << std::dec << '\n';
    std::uint32_t y = 0b1011;
    std::cout << "parity of 1011: " << (std::popcount(y) & 1) << " (odd number of ones), reverse of 8-bit 00000110: ";
    unsigned b = 0b00000110, r = 0;
    for (int i = 0; i < 8; ++i) { r = r << 1 | (b & 1); b >>= 1; }
    std::cout << r << " (01100000)\n";
}
