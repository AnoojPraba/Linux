#include <iostream>
#include <optional>
#include <variant>
#include <any>
#include <string>

#define DIVISOR_ZERO 0

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
std::optional<int> safeDivide(int numerator, int denominator)
{
    if (denominator == DIVISOR_ZERO)
    {
        return std::nullopt;
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
    std::optional<int> result = safeDivide(10, 2);
    if (result)
    {
        std::cout << "10 / 2 = " << *result << "\n";
    }

    std::optional<int> badResult = safeDivide(10, DIVISOR_ZERO);
    std::cout << "10 / 0 has value: " << badResult.has_value() << "\n";

    std::variant<int, std::string> variantValue = 5;
    std::cout << "variant holds int: " << std::get<int>(variantValue) << "\n";
    variantValue = std::string("now a string");
    std::cout << "variant holds string: " << std::get<std::string>(variantValue) << "\n";

    std::any anyValue = 42;
    std::cout << "any holds int: " << std::any_cast<int>(anyValue) << "\n";
    anyValue = std::string("now any holds a string");
    std::cout << std::any_cast<std::string>(anyValue) << "\n";

    return 0;
}
