#include <iostream>
#include <type_traits>

#define FACTORIAL_INPUT 5

using namespace std;

/*****************************************************************************
 * Name: factorial
 *
 * Description:
 *         Computes a factorial at compile time when given a constant
 *         expression argument (or at runtime otherwise).
 *
 * Inputs:
 *         n : the value to compute the factorial of.
 *
 * Returns:
 *         n factorial.
 *****************************************************************************/
constexpr long factorial(int n)
{
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

/*****************************************************************************
 * Name: describe
 *
 * Description:
 *         Prints whether the given value's type is integral or floating
 *         point, choosing the branch at compile time with if constexpr so
 *         only the taken branch is instantiated.
 *
 * Inputs:
 *         value : the value whose type is inspected.
 *
 * Returns:
 *         None.
 *****************************************************************************/
template <typename T>
void describe(const T &value)
{
    if constexpr (is_integral_v<T>)
    {
        cout << value << " is integral\n";
    }
    else if constexpr (is_floating_point_v<T>)
    {
        cout << value << " is floating point\n";
    }
    else
    {
        cout << "value is some other type\n";
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises a constexpr compile-time factorial and an if-constexpr
 *         templated dispatch function.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    constexpr long compileTimeFactorial = factorial(FACTORIAL_INPUT);
    cout << FACTORIAL_INPUT << "! = " << compileTimeFactorial << " (computed at compile time)\n";

    describe(42);
    describe(3.14);
    describe("text");

    return 0;
}
