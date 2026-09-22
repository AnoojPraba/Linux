#include <iostream>

/*****************************************************************************
 * Name: incrementViaReference
 *
 * Description:
 *         Increments a value through an lvalue reference parameter, modifying
 *         the caller's original variable.
 *
 * Inputs:
 *         value : lvalue reference to the int to increment.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void incrementViaReference(int &value)
{
    value += 1;
}

/*****************************************************************************
 * Name: incrementViaPointer
 *
 * Description:
 *         Increments a value through a pointer parameter, requiring explicit
 *         dereferencing and a valid (non-null) address.
 *
 * Inputs:
 *         value : pointer to the int to increment.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void incrementViaPointer(int *value)
{
    if (value != nullptr)
    {
        *value += 1;
    }
}

/*****************************************************************************
 * Name: describeRvalue
 *
 * Description:
 *         Accepts an rvalue reference, demonstrating binding to a temporary.
 *
 * Inputs:
 *         value : rvalue reference to an int temporary.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void describeRvalue(int &&value)
{
    std::cout << "rvalue reference bound to temporary: " << value << "\n";
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates lvalue references, pointers, and rvalue references,
 *         contrasting reference syntax with pointer syntax.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    int counter = 0;

    incrementViaReference(counter);
    std::cout << "after reference increment: " << counter << "\n";

    incrementViaPointer(&counter);
    std::cout << "after pointer increment: " << counter << "\n";

    int &aliasOfCounter = counter;
    aliasOfCounter += 10;
    std::cout << "after alias modification: " << counter << "\n";

    describeRvalue(42);
    return 0;
}
