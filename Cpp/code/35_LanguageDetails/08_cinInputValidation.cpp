// cin fail state: clear() + ignore() loop (reads stdin).

#include <iostream>
#include <limits>

int read_positive()
{
    int n;
    while (true)
    {
        std::cout << "Enter a positive number: ";
        if (std::cin >> n && n > 0)                       // extraction succeeded and the value is valid
            return n;
        if (std::cin.eof())
            return -1;                                    // no more input at all
        std::cin.clear();                                 // 1. clear the fail state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   // 2. discard the rest of the line
        std::cout << "invalid input, try again\n";
    }
}

int main()
{
    int a = read_positive();
    int b = read_positive();
    std::cout << "\nYou entered " << a << " and " << b << ", sum = " << a + b << '\n';
}
