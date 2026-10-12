// strlen/sizeof, strcat/strncpy/strcmp/strchr/strstr, std::string +, +=,
// append.

#include <cstring>
#include <iostream>
#include <string>

int main()
{
    char a[20] = "Hello";                          // 5 characters + '\0' in an array of 20
    char b[] = "World";                            // the size is deduced: 6
    std::cout << "strlen(a) = " << std::strlen(a) << ", sizeof(a) = " << sizeof(a) << ", sizeof(b) = " << sizeof(b) << '\n';

    std::strcat(a, " ");                           // append: the destination must have room
    std::strcat(a, b);
    std::cout << "after strcat: " << a << '\n';

    char copy[20];
    std::strncpy(copy, a, sizeof copy - 1);        // bounded copy
    copy[sizeof copy - 1] = '\0';                  // strncpy does not always terminate: do it yourself
    std::cout << "copy: " << copy << ", strcmp(a, copy) = " << std::strcmp(a, copy) << '\n';
    std::cout << "strcmp(\"apple\", \"banana\") < 0: " << (std::strcmp("apple", "banana") < 0) << '\n';
    std::cout << "first 'o' at index " << (std::strchr(a, 'o') - a) << ", \"World\" at index " << (std::strstr(a, "World") - a) << '\n';

    // std::string: no buffer sizes to manage
    std::string first = "Hello", second = "World";
    std::string joined = first + " " + second;     // + concatenates
    joined += "!";                                 // += appends
    joined.append(" Bye");
    std::cout << joined << " (length " << joined.size() << ")\n";
    // "Hello" + " " + "World" would not compile: + needs at least one std::string operand
    std::cout << std::boolalpha << "first == \"Hello\": " << (first == "Hello") << ", first < second: " << (first < second) << '\n';

    const char *c = joined.c_str();                // a view of the string for C APIs
    std::cout << "c_str(): " << c << ", std::string(a): " << std::string(a) << '\n';
}
