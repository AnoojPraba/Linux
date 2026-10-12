// std::bitset and vector<bool>: set/reset/flip/test, counting, shifts,
// conversions and bitset as a compact set of small integers.

#include <bitset>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::bitset<8> b("00101101");
    std::cout << "b = " << b << ", count = " << b.count() << ", size = " << b.size() << ", any/none/all = " << b.any() << b.none() << b.all() << '\n';
    b.set(1).reset(0).flip(7);                                       // chainable; flip() with no argument flips every bit
    std::cout << "after set(1), reset(0), flip(7): " << b << ", test(7) = " << b.test(7) << ", b[2] = " << b[2] << '\n';
    std::cout << "shifts: " << (b << 2) << ' ' << (b >> 3) << ", ~b = " << ~b << ", b & mask = " << (b & std::bitset<8>(0x0F)) << '\n';
    std::cout << "to_ulong = " << b.to_ulong() << ", to_string = " << b.to_string() << ", from 77: " << std::bitset<8>(77) << '\n';

    std::bitset<100> seen;                                           // a set of integers in [0, 100): one bit each
    for (int x : {3, 14, 15, 92, 65, 35, 3, 14}) seen.set(x);
    std::cout << "distinct values seen: " << seen.count() << ", 14 present: " << seen.test(14) << ", 50 present: " << seen.test(50) << '\n';

    std::bitset<16> a(0b1100'1010'1111'0000u), c(0b1010'1010'1010'1010u);
    std::cout << "hamming distance = " << (a ^ c).count() << ", intersection size = " << (a & c).count() << ", union size = " << (a | c).count() << '\n';

    std::vector<bool> v(10, false);                                  // packed bits, but its reference proxy is NOT a real bool&
    v[3] = true;
    v.flip();
    std::cout << "vector<bool> after flip:";
    for (bool x : v) std::cout << x;
    std::cout << " (use vector<char> or bitset when you need real references)\n";
    try { std::bitset<4>("12"); } catch (const std::invalid_argument &) { std::cout << "bitset(\"12\") throws invalid_argument\n"; }
}
