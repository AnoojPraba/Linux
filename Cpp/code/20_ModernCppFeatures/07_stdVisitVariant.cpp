#include <iostream>
#include <string>
#include <variant>

using namespace std;

// The "overloaded lambda set" helper: combines several callables into one
// object with multiple operator() overloads, so std::visit can pick the
// right one by argument type.
template<class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates std::visit over a std::variant<int, double,
 *         std::string> using the overloaded-lambda-set idiom to dispatch on
 *         the currently held alternative without a manual if/else chain.
 *
 *         This contrasts with the classic OOP Visitor pattern in
 *         DesignPatterns/Behavioral/Visitor.cpp: both achieve type-based
 *         dispatch without a big if/else chain, but std::visit works over a
 *         closed set of types known at compile time via the variant, while
 *         the OOP Visitor pattern works over an open/extensible class
 *         hierarchy (new element types can be added without touching the
 *         variant-based code, but new variant alternatives require every
 *         std::visit call site to be updated).
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    variant<int, double, string> value = 42;

    auto printer = overloaded{
        [](int v) { cout << "int: " << v << endl; },
        [](double v) { cout << "double: " << v << endl; },
        [](const string& v) { cout << "string: " << v << endl; }
    };

    visit(printer, value);

    value = 3.14;
    visit(printer, value);

    value = string("hello variant");
    visit(printer, value);

    return 0;
}
