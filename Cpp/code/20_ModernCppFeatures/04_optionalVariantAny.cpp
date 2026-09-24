#include <iostream>
#include <optional>
#include <variant>
#include <any>
#include <string>

#define DIVISOR_ZERO 0

using namespace std;

/*****************************************************************************
 * Name: safeDivide
 *
 * Description:
 *         Divides two integers, returning an empty optional instead of
 *         throwing or invoking undefined behavior when denominator is zero.
 *
 * Inputs:
 *         numerator   : the value to divide.
 *         denominator : the value to divide by.
 *
 * Returns:
 *         The quotient wrapped in std::optional, or std::nullopt if
 *         denominator is zero.
 *****************************************************************************/
optional<int> safeDivide(int numerator, int denominator)
{
    if (denominator == DIVISOR_ZERO)
    {
        return nullopt;
    }
    return numerator / denominator;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates std::optional for "may not have a value", std::variant
 *         for a type-safe tagged union, and std::any for a type-erased
 *         container holding any copyable type.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    optional<int> result = safeDivide(10, 2);
    if (result)
    {
        cout << "10 / 2 = " << *result << "\n";
    }

    optional<int> badResult = safeDivide(10, DIVISOR_ZERO);
    cout << "10 / 0 has value: " << badResult.has_value() << "\n";

    variant<int, string> variantValue = 5;
    cout << "variant holds int: " << get<int>(variantValue) << "\n";
    variantValue = string("now a string");
    cout << "variant holds string: " << get<string>(variantValue) << "\n";

    any anyValue = 42;
    cout << "any holds int: " << any_cast<int>(anyValue) << "\n";
    anyValue = string("now any holds a string");
    cout << any_cast<string>(anyValue) << "\n";

    return 0;
}
