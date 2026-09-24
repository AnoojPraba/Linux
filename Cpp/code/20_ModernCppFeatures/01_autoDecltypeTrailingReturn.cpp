#include <iostream>

using namespace std;

/*****************************************************************************
 * Name: addValues
 *
 * Description:
 *         Adds two values of possibly different types, using a trailing
 *         return type deduced with decltype so the return type can depend
 *         on both parameter types.
 *
 * Inputs:
 *         a : first operand.
 *         b : second operand.
 *
 * Returns:
 *         The sum of a and b, typed as decltype(a + b).
 *****************************************************************************/
template <typename A, typename B>
auto addValues(A a, B b) -> decltype(a + b)
{
    return a + b;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates auto type deduction, decltype, and a trailing return
 *         type template function.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    auto intValue = 5;
    auto doubleValue = 2.5;

    decltype(intValue) anotherInt = 10;
    cout << "intValue = " << intValue << ", anotherInt = " << anotherInt << "\n";

    auto mixedSum = addValues(intValue, doubleValue);
    cout << "mixedSum (int + double) = " << mixedSum << "\n";

    return 0;
}
