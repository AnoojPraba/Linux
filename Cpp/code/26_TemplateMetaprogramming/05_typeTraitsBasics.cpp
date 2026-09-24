#include <iostream>
#include <type_traits>

using namespace std;

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises basic type traits: std::is_same to compare types at
 *         compile time, std::enable_if to constrain a template, and
 *         std::conditional to pick between two types based on a compile-
 *         time boolean.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    cout << "is_same<int, int> = " << is_same<int, int>::value << "\n";
    cout << "is_same<int, double> = " << is_same<int, double>::value << "\n";

    // std::conditional<Condition, TrueType, FalseType>::type picks a type, not a value,
    // entirely at compile time.
    using SelectedType = conditional<true, int, double>::type;
    static_assert(is_same<SelectedType, int>::value, "expected int to be selected");
    SelectedType selected = 7;
    cout << "conditional selected type holds: " << selected << "\n";

    cout << "is_pointer<int*> = " << is_pointer<int *>::value << "\n";
    cout << "is_const<const int> = " << is_const<const int>::value << "\n";

    return 0;
}
