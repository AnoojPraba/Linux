#include <iostream>
#include <type_traits>

// C++20 concepts (e.g. `template <std::integral T> T add(T a, T b)`) are not available
// on this toolchain (GCC 8.5.0 lacks <concepts> even under -std=c++2a; see NOTES.md in
// this folder). This file shows the equivalent C++17 constraint using enable_if plus a
// static_assert for a clearer error message, which is what concepts replace.

/*****************************************************************************
 * Name: add
 *
 * Description:
 *         Adds two values, constrained to arithmetic types only. In C++20
 *         this would be written as
 *         `template <std::integral T> T add(T a, T b)` using a concept;
 *         here the constraint is enforced with enable_if plus a
 *         static_assert that produces a readable error on misuse.
 *
 * Inputs:
 *         a : first operand.
 *         b : second operand.
 *
 * Returns:
 *         The sum of a and b.
 *****************************************************************************/
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
T add(T a, T b)
{
    static_assert(std::is_arithmetic<T>::value, "add() requires an arithmetic type");
    return a + b;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Calls the constrained add() with arithmetic types to show the
 *         C++17 concepts-equivalent in action.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::cout << "add(2, 3) = " << add(2, 3) << "\n";
    std::cout << "add(2.5, 1.5) = " << add(2.5, 1.5) << "\n";

    return 0;
}
