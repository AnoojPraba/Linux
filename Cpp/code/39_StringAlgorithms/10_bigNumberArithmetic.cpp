// Arbitrary-precision integers stored as strings/vectors: addition,
// subtraction, multiplication and factorial.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::string add(const std::string &a, const std::string &b)
{
    std::string out;
    int carry = 0;
    for (long i = static_cast<long>(a.size()) - 1, j = static_cast<long>(b.size()) - 1; i >= 0 || j >= 0 || carry; --i, --j)
    {
        int s = carry + (i >= 0 ? a[i] - '0' : 0) + (j >= 0 ? b[j] - '0' : 0);
        out += static_cast<char>('0' + s % 10);
        carry = s / 10;
    }
    std::reverse(out.begin(), out.end());
    return out;
}

std::string multiply(const std::string &a, const std::string &b)
{
    std::vector<int> res(a.size() + b.size(), 0);
    for (long i = static_cast<long>(a.size()) - 1; i >= 0; --i)
        for (long j = static_cast<long>(b.size()) - 1; j >= 0; --j)
        {
            int p = (a[i] - '0') * (b[j] - '0') + res[i + j + 1];       // digit i * digit j lands at i + j + 1
            res[i + j + 1] = p % 10;
            res[i + j] += p / 10;
        }
    std::string out;
    std::size_t k = 0;
    while (k + 1 < res.size() && res[k] == 0) ++k;
    for (; k < res.size(); ++k) out += static_cast<char>('0' + res[k]);
    return out;
}

std::string factorial(int n)
{
    std::vector<int> d{1};                                   // little-endian digits
    for (int x = 2; x <= n; ++x)
    {
        int carry = 0;
        for (int &digit : d) { int p = digit * x + carry; digit = p % 10; carry = p / 10; }
        for (; carry; carry /= 10) d.push_back(carry % 10);
    }
    std::string out;
    for (auto it = d.rbegin(); it != d.rend(); ++it) out += static_cast<char>('0' + *it);
    return out;
}

int main()
{
    std::cout << "99999999999999999999 + 1 = " << add("99999999999999999999", "1") << '\n';
    std::cout << "123456789 * 987654321 = " << multiply("123456789", "987654321") << '\n';
    std::cout << "(10^20 - 1)^2 = " << multiply("99999999999999999999", "99999999999999999999") << '\n';
    std::cout << "30! = " << factorial(30) << '\n';
    std::cout << "for speed use base 10^9 limbs; Karatsuba multiplies in O(n^1.58); production code uses GMP or Boost.Multiprecision\n";
}
