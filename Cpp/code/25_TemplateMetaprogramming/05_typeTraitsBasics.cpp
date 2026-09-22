#include <iostream>
#include <type_traits>

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
    std::cout << "is_same<int, int> = " << std::is_same<int, int>::value << "\n";
    std::cout << "is_same<int, double> = " << std::is_same<int, double>::value << "\n";

    // std::conditional<Condition, TrueType, FalseType>::type picks a type, not a value,
    // entirely at compile time.
    using SelectedType = std::conditional<true, int, double>::type;
    static_assert(std::is_same<SelectedType, int>::value, "expected int to be selected");
    SelectedType selected = 7;
    std::cout << "conditional selected type holds: " << selected << "\n";

    std::cout << "is_pointer<int*> = " << std::is_pointer<int *>::value << "\n";
    std::cout << "is_const<const int> = " << std::is_const<const int>::value << "\n";

    return 0;
}
