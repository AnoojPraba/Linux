// sqrt, pow, floor/ceil/round/trunc on +/- 2.5, fmod, trig/log, integer vs
// floating division, NaN/inf tests.

#include <cmath>
#include <iomanip>
#include <iostream>

int main()
{
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "sqrt(16) = " << std::sqrt(16.0) << ", cbrt(27) = " << std::cbrt(27.0) << '\n';
    std::cout << "pow(2, 10) = " << std::pow(2.0, 10) << ", pow(9, 0.5) = " << std::pow(9.0, 0.5) << '\n';
    std::cout << "abs(-7.5) = " << std::abs(-7.5) << ", fmod(10.5, 3) = " << std::fmod(10.5, 3.0) << '\n';

    double x = 2.5, y = -2.5;
    std::cout << "floor(2.5) = " << std::floor(x) << ", ceil(2.5) = " << std::ceil(x)
              << ", round(2.5) = " << std::round(x) << ", trunc(2.5) = " << std::trunc(x) << '\n';
    std::cout << "floor(-2.5) = " << std::floor(y) << ", ceil(-2.5) = " << std::ceil(y)
              << ", round(-2.5) = " << std::round(y) << ", trunc(-2.5) = " << std::trunc(y) << '\n';

    const double pi = std::acos(-1.0);
    std::cout << "pi = " << std::setprecision(5) << pi << ", sin(pi/2) = " << std::sin(pi / 2) << ", cos(0) = " << std::cos(0.0) << '\n';
    std::cout << std::setprecision(2);
    std::cout << "exp(1) = " << std::exp(1.0) << ", log(e) = " << std::log(std::exp(1.0)) << ", log10(1000) = " << std::log10(1000.0)
              << ", log2(8) = " << std::log2(8.0) << '\n';
    std::cout << "hypot(3, 4) = " << std::hypot(3.0, 4.0) << '\n';

    std::cout << "7 / 2 = " << 7 / 2 << ", 7 / 2.0 = " << 7 / 2.0 << ", 7 % 3 = " << 7 % 3 << ", -7 % 3 = " << -7 % 3 << '\n';
    std::cout << std::boolalpha << "sqrt(-1) is nan: " << std::isnan(std::sqrt(-1.0)) << ", 1/0.0 is inf: " << std::isinf(1.0 / 0.0) << '\n';
}
