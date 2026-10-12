// Powers of two and logarithms with bit scans: is power of 2/4, next power of
// two, floor(log2), bit length, integer square root and fast exponentiation by
// squaring.

#include <bit>
#include <cstdint>
#include <iostream>

constexpr bool is_pow4(std::uint32_t x) { return std::has_single_bit(x) && (x & 0x55555555); }

constexpr std::uint32_t isqrt(std::uint32_t n)                       // bitwise integer square root, no floating point
{
    std::uint32_t res = 0, bit = 1u << 30;
    while (bit > n) bit >>= 2;
    while (bit)
    {
        if (n >= res + bit) { n -= res + bit; res = (res >> 1) + bit; }
        else res >>= 1;
        bit >>= 2;
    }
    return res;
}

constexpr std::uint64_t power_mod(std::uint64_t base, std::uint64_t exp, std::uint64_t mod)   // exponentiation by squaring, O(log exp)
{
    std::uint64_t r = 1;
    for (base %= mod; exp; exp >>= 1, base = base * base % mod) if (exp & 1) r = r * base % mod;
    return r;
}

int main()
{
    static_assert(isqrt(1000000) == 1000 && is_pow4(16) && !is_pow4(8));    // evaluated at compile time
    for (std::uint32_t x : {1u, 3u, 16u, 100u, 1024u, 1000000u})
        std::cout << x << ": pow2=" << std::has_single_bit(x) << " pow4=" << is_pow4(x) << " bit_ceil=" << std::bit_ceil(x) << " floor_log2=" << std::bit_width(x) - 1
                  << " isqrt=" << isqrt(x) << '\n';
    std::cout << "3^200 mod 1e9+7 = " << power_mod(3, 200, 1'000'000'007) << ", 2^62 mod 1e18+9 = " << power_mod(2, 62, 1'000'000'000'000'000'009ULL) << '\n';
    std::cout << "13 << 3 = " << (13 << 3) << ", 100 >> 2 = " << (100 >> 2) << ", n % 2^k = n & (2^k - 1): " << (77 % 8) << " == " << (77 & 7) << '\n';
}
