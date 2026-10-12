// Plain enum vs enum class, precedence pitfalls (x & 1 == 0, 1 << 2 + 1).

#include <iostream>

enum Color { Red, Green = 5, Blue };                  // plain enum: Red = 0, Green = 5, Blue = 6
enum class Level : unsigned char { Low = 1, High = 10 };   // scoped, with a chosen underlying type

int main()
{
    Color c = Blue;
    int n = c;                                         // plain enum converts to int implicitly
    std::cout << "Blue = " << n << ", Red = " << Red << '\n';

    Level l = Level::High;                             // the name must be qualified
    // int bad = l;                                    // error: no implicit conversion
    int ok = static_cast<int>(l);
    std::cout << "Level::High = " << ok << ", sizeof(Level) = " << sizeof(Level) << '\n';

    int a = 2, b = 3, d = 4;
    std::cout << "a + b * d = " << a + b * d << ", (a + b) * d = " << (a + b) * d << '\n';
    std::cout << "10 - 4 - 3 = " << 10 - 4 - 3 << " (left to right), a = b = d assigns right to left: ";
    a = b = d;
    std::cout << a << ' ' << b << '\n';

    bool t = true, f = false;
    std::cout << std::boolalpha << "t || f && f = " << (t || f && f) << " (&& binds tighter than ||)\n";

    int x = 6;
    std::cout << "x & 1 == 0 is " << (x & 1 == 0) << " (== binds tighter than &), (x & 1) == 0 is " << ((x & 1) == 0) << '\n';
    int i = 5;
    std::cout << "-i * 2 = " << -i * 2 << ", !0 + 1 = " << !0 + 1 << ", 1 << 2 + 1 = " << (1 << 2 + 1) << " (+ binds tighter than <<)\n";
}
