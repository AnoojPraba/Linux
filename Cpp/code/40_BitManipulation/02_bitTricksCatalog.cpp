// A catalog of one-line bit tricks in C++: lowest set bit, clear lowest,
// branch-free min/max/abs, sign, swap, average without overflow and field
// extraction.

#include <bitset>
#include <cstdint>
#include <iostream>

template <std::size_t N = 8>
void show(const char *label, std::uint32_t x) { std::cout << label << " = " << std::bitset<N>(x) << " (" << x << ")\n"; }

int main()
{
    std::uint32_t x = 0b01101000;
    show("x                        ", x);
    show("x & -x   (lowest set bit)", x & -x);
    show("x & (x-1) (clear lowest) ", x & (x - 1));
    show("x | (x-1) (set trailing) ", x | (x - 1));
    show("~x & (x+1) (lowest zero) ", ~x & (x + 1));
    show("x ^ (x>>1) (Gray code)   ", x ^ (x >> 1));

    int a = 7, b = -12;
    int mask = (a - b) >> 31;                                       // all ones if a < b (arithmetic shift of a negative: implementation-defined before C++20, defined in C++20)
    std::cout << "branch-free min " << (b + ((a - b) & mask)) << ", max " << (a - ((a - b) & mask)) << ", abs(-12) " << ((b ^ (b >> 31)) - (b >> 31)) << '\n';

    unsigned p = 4'000'000'000u, q = 3'000'000'000u;
    std::cout << "average without overflow: " << ((p & q) + ((p ^ q) >> 1)) << '\n';

    std::uint32_t reg = 0xABCD1234;
    std::cout << std::hex << "bits 8..15: 0x" << ((reg >> 8) & 0xFF) << ", with bits 8..15 replaced by 0xFF: 0x" << ((reg & ~(0xFFu << 8)) | (0xFFu << 8)) << std::dec << '\n';
    std::cout << "round 37 up to a multiple of 8: " << ((37 + 7) & ~7) << ", down: " << (37 & ~7) << ", 77 % 8 == 77 & 7: " << (77 % 8 == (77 & 7)) << '\n';

    a ^= b; b ^= a; a ^= b;
    std::cout << "xor swap: a = " << a << ", b = " << b << " (fails if both refer to the same object)\n";
    std::cout << "is power of two: " << std::boolalpha << (64 && !(64 & 63)) << ", " << (96 && !(96 & 95)) << '\n';
}
