// Mangled vs demangled type names, extern "C" linkage and overloads.

#include <cxxabi.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <typeinfo>

extern "C" int c_add(int a, int b) { return a + b; }       // C linkage: the symbol is exactly "c_add"

int add(int a, int b) { return a + b; }                    // C++ linkage: encodes the parameter types
double add(double a, double b) { return a + b; }           // legal only because the symbol names differ

std::string demangle(const char *mangled)
{
    int status = 0;
    char *p = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
    std::string s = status == 0 ? p : mangled;
    std::free(p);
    return s;
}

namespace shapes { struct Circle {}; }

int main()
{
    // The mangled form of a type is what typeid().name() returns on GCC and Clang.
    std::cout << typeid(int (*)(int, int)).name() << "  ->  " << demangle(typeid(int (*)(int, int)).name()) << '\n';
    std::cout << typeid(shapes::Circle).name() << "  ->  " << demangle(typeid(shapes::Circle).name()) << '\n';
    std::cout << typeid(double (*)(double, double)).name() << "  ->  "
              << demangle(typeid(double (*)(double, double)).name()) << '\n';

    // A function with C linkage can be called from C code and looked up by name in a shared library.
    int (*cf)(int, int) = c_add;
    std::cout << cf(2, 3) << ' ' << add(2, 3) << ' ' << add(1.5, 2.5) << '\n';
}
