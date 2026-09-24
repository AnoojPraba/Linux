#include <iostream>
#include <type_traits>

using namespace std;

/*****************************************************************************
 * Name: describeNumber
 *
 * Description:
 *         Overload selected via SFINAE (Substitution Failure Is Not An
 *         Error) only when T is an integral type; the compiler removes this
 *         overload from consideration for non-integral T instead of erroring.
 *
 * Inputs:
 *         value : the integral value to describe.
 *
 * Returns:
 *         None.
 *****************************************************************************/
template <typename T, typename enable_if<is_integral<T>::value, int>::type = 0>
void describeNumber(T value)
{
    cout << value << " is integral\n";
}

/*****************************************************************************
 * Name: describeNumber
 *
 * Description:
 *         Overload selected via SFINAE only when T is a floating point type.
 *
 * Inputs:
 *         value : the floating point value to describe.
 *
 * Returns:
 *         None.
 *****************************************************************************/
template <typename T, typename enable_if<is_floating_point<T>::value, int>::type = 0>
void describeNumber(T value)
{
    cout << value << " is floating point\n";
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Calls the SFINAE-selected describeNumber overload for both an
 *         integral and a floating point argument.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    describeNumber(42);
    describeNumber(3.14);

    return 0;
}
