// Arithmetic from bit operations only: add, subtract, multiply, divide and
// compare, with unsigned types to avoid undefined behaviour.

#include <cstdint>
#include <iostream>

std::int32_t add(std::int32_t a, std::int32_t b)
{
    std::uint32_t x = static_cast<std::uint32_t>(a), y = static_cast<std::uint32_t>(b);   // unsigned arithmetic wraps: no UB
    while (y)
    {
        std::uint32_t carry = (x & y) << 1;                          // both bits 1: a carry moves left
        x ^= y;                                                      // sum without the carries
        y = carry;
    }
    return static_cast<std::int32_t>(x);
}

std::int32_t negate(std::int32_t a) { return add(~a, 1); }
std::int32_t subtract(std::int32_t a, std::int32_t b) { return add(a, negate(b)); }

std::int32_t multiply(std::int32_t a, std::int32_t b)
{
    bool negative = (a < 0) != (b < 0);
    std::uint32_t x = static_cast<std::uint32_t>(a < 0 ? negate(a) : a), y = static_cast<std::uint32_t>(b < 0 ? negate(b) : b);
    std::int32_t result = 0;
    for (; y; y >>= 1, x <<= 1)                                      // shift-and-add: long multiplication in binary
        if (y & 1) result = add(result, static_cast<std::int32_t>(x));
    return negative ? negate(result) : result;
}

std::int32_t divide(std::int32_t a, std::int32_t b)
{
    bool negative = (a < 0) != (b < 0);
    std::uint32_t x = static_cast<std::uint32_t>(a < 0 ? negate(a) : a), y = static_cast<std::uint32_t>(b < 0 ? negate(b) : b), q = 0;
    for (int i = 31; i >= 0; --i)                                    // binary long division
        if ((x >> i) >= y) { x -= y << i; q |= 1u << i; }
    return negative ? negate(static_cast<std::int32_t>(q)) : static_cast<std::int32_t>(q);
}

int main()
{
    std::cout << "add(23, 19) = " << add(23, 19) << ", add(-5, 8) = " << add(-5, 8) << ", subtract(10, 25) = " << subtract(10, 25) << '\n';
    std::cout << "multiply(7, -6) = " << multiply(7, -6) << ", multiply(-9, -9) = " << multiply(-9, -9) << '\n';
    std::cout << "divide(100, 7) = " << divide(100, 7) << ", divide(-100, 7) = " << divide(-100, 7) << '\n';
    std::cout << "equal: " << std::boolalpha << !(42 ^ 42) << ", " << !(42 ^ 43) << '\n';
}
