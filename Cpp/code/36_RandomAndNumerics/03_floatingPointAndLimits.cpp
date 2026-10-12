// 0.1+0.2, abs+relative tolerance compare, numeric_limits, NaN/inf, signed
// overflow check, midpoint/lerp.

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>

bool nearly_equal(double a, double b, double rel = 1e-9, double abs_tol = 1e-12)
{
    return std::abs(a - b) <= std::max(abs_tol, rel * std::max(std::abs(a), std::abs(b)));
}

bool add_overflows(int a, int b)                          // check BEFORE adding: signed overflow is undefined behavior
{
    return (b > 0 && a > std::numeric_limits<int>::max() - b) || (b < 0 && a < std::numeric_limits<int>::min() - b);
}

int main()
{
    std::cout << std::boolalpha << std::setprecision(17);
    double x = 0.1 + 0.2;
    std::cout << "0.1 + 0.2 = " << x << ", == 0.3: " << (x == 0.3) << ", nearly equal: " << nearly_equal(x, 0.3) << '\n';

    std::cout << "int range: " << std::numeric_limits<int>::min() << " .. " << std::numeric_limits<int>::max() << '\n';
    std::cout << "double epsilon: " << std::numeric_limits<double>::epsilon() << ", digits10: " << std::numeric_limits<double>::digits10 << '\n';
    std::cout << "uint8_t max: " << static_cast<int>(std::numeric_limits<std::uint8_t>::max()) << ", char is signed: "
              << std::numeric_limits<char>::is_signed << '\n';

    double nan = std::numeric_limits<double>::quiet_NaN(), inf = std::numeric_limits<double>::infinity();
    std::cout << "nan == nan: " << (nan == nan) << ", isnan: " << std::isnan(nan) << ", 1/inf: " << 1.0 / inf << ", inf - inf is nan: " << std::isnan(inf - inf) << '\n';

    std::cout << "INT_MAX + 1 overflows: " << add_overflows(std::numeric_limits<int>::max(), 1) << '\n';
    unsigned u = 0;
    std::cout << "unsigned 0 - 1 wraps to " << u - 1 << " (well defined); signed overflow is not\n";

    std::cout << "midpoint(INT_MAX, INT_MAX-2) = " << std::midpoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max() - 2)
              << " (no overflow), lerp(10, 20, 0.25) = " << std::lerp(10.0, 20.0, 0.25) << '\n';
    float f = 16777216.0f;
    std::cout << "float 2^24 + 1 == 2^24: " << (f + 1.0f == f) << " (a float has only 24 bits of precision)\n";
    std::cout << "ints converted to double lose nothing up to 2^53: " << (static_cast<double>(9007199254740992LL) + 1 == 9007199254740992.0) << '\n';
}
