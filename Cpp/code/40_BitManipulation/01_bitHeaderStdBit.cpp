// The C++20 <bit> header: popcount, countl_zero/countr_zero, bit_width,
// bit_ceil/bit_floor, has_single_bit, rotl/rotr, bit_cast and endian.

#include <bit>
#include <cstdint>
#include <iostream>

int main()
{
    std::uint32_t x = 0b0000'0000'0000'0000'0001'0110'0100'0000;     // digit separators make binary literals readable
    std::cout << std::boolalpha;
    std::cout << "popcount            : " << std::popcount(x) << '\n';
    std::cout << "countl_zero (leading) : " << std::countl_zero(x) << ", countr_zero (trailing): " << std::countr_zero(x) << '\n';
    std::cout << "bit_width (bits needed): " << std::bit_width(x) << " = floor(log2 x) + 1\n";
    std::cout << "bit_floor / bit_ceil  : " << std::bit_floor(x) << " / " << std::bit_ceil(x) << " (largest / smallest power of two around x)\n";
    std::cout << "has_single_bit(64): " << std::has_single_bit(64u) << ", (96): " << std::has_single_bit(96u) << ", (0): " << std::has_single_bit(0u) << '\n';
    std::cout << "rotl(0x80000001, 1) = 0x" << std::hex << std::rotl(0x80000001u, 1) << ", rotr(0x80000001, 1) = 0x" << std::rotr(0x80000001u, 1) << std::dec << '\n';

    float f = 1.0f;
    auto bits = std::bit_cast<std::uint32_t>(f);                    // reinterpret an object's bytes without undefined behaviour
    std::cout << "bit pattern of 1.0f: 0x" << std::hex << bits << std::dec << " (sign 0, exponent " << (bits >> 23 & 255) << ", mantissa " << (bits & 0x7FFFFF) << ")\n";
    std::cout << "this machine is " << (std::endian::native == std::endian::little ? "little" : "big") << " endian\n";
    std::cout << "countr_zero(0) = " << std::countr_zero(0u) << " (defined: the width), unlike the old __builtin_ctz(0)\n";
}
