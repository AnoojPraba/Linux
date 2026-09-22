#include <iostream>

/*****************************************************************************
 * Name: sumAll
 *
 * Description:
 *         Sums an arbitrary number of arguments using a C++17 unary fold
 *         expression over the '+' operator.
 *
 * Inputs:
 *         args : the parameter pack of values to sum.
 *
 * Returns:
 *         The sum of every argument in args.
 *****************************************************************************/
template <typename... Args>
auto sumAll(Args... args)
{
    return (args + ...);
}

/*****************************************************************************
 * Name: printAll
 *
 * Description:
 *         Prints every argument in the parameter pack separated by spaces,
 *         using a fold expression over the comma operator.
 *
 * Inputs:
 *         args : the parameter pack of values to print.
 *
 * Returns:
 *         None.
 *****************************************************************************/
template <typename... Args>
void printAll(Args... args)
{
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates variadic template parameter packs combined with
 *         C++17 fold expressions for both a numeric fold and a printing
 *         fold.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::cout << "sumAll(1, 2, 3, 4) = " << sumAll(1, 2, 3, 4) << "\n";
    std::cout << "sumAll(1.5, 2.5) = " << sumAll(1.5, 2.5) << "\n";
    printAll("hello", 42, 3.14, 'c');

    return 0;
}
