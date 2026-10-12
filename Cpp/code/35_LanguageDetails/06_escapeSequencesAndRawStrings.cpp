// \n \t \\ \" \0 \x41, string with embedded NUL, raw string literals.

#include <iostream>
#include <string>

int main()
{
    std::cout << "Line one\nLine two\n";                 // \n  newline
    std::cout << "Name\tScore\nAnn\t90\n";               // \t  tab
    std::cout << "She said \"hello\"\n";                 // \"  double quote
    std::cout << "It's a single quote: \'\n";            // \'  single quote
    std::cout << "A backslash: \\\n";                    // \\  backslash
    std::cout << "Hex 0x41 is \x41 and octal 101 is \101\n";   // \xhh and \ooo give a character by code

    std::string cut("ab\0cd");                           // \0 is the null character: a C string ends there
    std::string whole("ab\0cd", 5);                      // giving the length keeps all five characters
    std::cout << "sizes: " << cut.size() << " and " << whole.size() << '\n';
    std::cout << "char codes: '\\n' = " << static_cast<int>('\n') << ", '\\t' = " << static_cast<int>('\t')
              << ", '\\0' = " << static_cast<int>('\0') << '\n';

    std::cout << R"(Raw string: C:\new\table "quoted" \n stays as typed)" << '\n';
}
