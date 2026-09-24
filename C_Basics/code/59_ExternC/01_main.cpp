#include <iostream>
#include "mathutils.h"

// This is a C++ translation unit calling into a C-compiled library
// through the extern "C" declarations in mathutils.h - without those,
// the linker would fail to find addTwo/squareOf at all, since this file
// would look for C++-mangled names that mathutils.c (compiled as plain C)
// never produced.
int main()
{
    std::cout << "addTwo(2, 3) = " << addTwo(2, 3) << std::endl;
    std::cout << "squareOf(5) = " << squareOf(5) << std::endl;

    return 0;
}
