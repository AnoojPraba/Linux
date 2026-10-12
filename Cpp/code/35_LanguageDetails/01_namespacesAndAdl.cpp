// Anonymous/inline/nested namespaces, aliases, using-declaration vs directive,
// argument-dependent lookup.

#include <iostream>
#include <string>

namespace app
{
    int counter = 0;                                  // external linkage: visible to other translation units

    namespace detail { int helper() { return 42; } }  // nested namespace (C++17: namespace app::detail)

    namespace
    {
        int file_local = 7;                           // anonymous namespace: internal linkage, private to this file
    }

    inline namespace v2                               // names are visible as app::name; v1 can be kept for old callers
    {
        std::string version() { return "v2"; }
    }
    namespace v1 { std::string version() { return "v1"; } }

    struct Point { int x, y; };
    std::ostream &operator<<(std::ostream &os, const Point &p) { return os << '(' << p.x << ',' << p.y << ')'; }
    void move(Point &p) { ++p.x; }
}

namespace lib = app::detail;                          // a namespace alias

namespace other { void move(int) { std::cout << "other::move\n"; } }

int main()
{
    std::cout << lib::helper() << ' ' << app::file_local << ' ' << app::version() << ' ' << app::v1::version() << '\n';

    app::Point p{1, 2};
    move(p);                                          // found by ADL: p's type lives in app, so app::move is also considered
    std::cout << p << '\n';                           // operator<< found by ADL, without writing app::

    using app::counter;                               // a using-declaration brings in exactly one name
    ++counter;
    std::cout << "counter = " << app::counter << '\n';
    {
        using namespace other;                        // a using-directive makes all names of 'other' visible here
        move(5);
    }
}
