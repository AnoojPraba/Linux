// do-while, switch fall-through, break/continue in nested loops.

#include <iostream>

int main()
{
    int n = 10;
    do
    {
        std::cout << "do-while body runs once even though n > 5: n = " << n << '\n';
    } while (n < 5);

    int i = 0;
    while (i < 5)
    {
        ++i;
        if (i == 2)
            continue;                      // skip the rest of this iteration
        if (i == 5)
            break;                         // leave the loop
        std::cout << "i = " << i << '\n';
    }

    for (int day = 6; day <= 8; ++day)
    {
        switch (day)
        {
        case 6:
        case 7:                            // two labels share one body
            std::cout << day << ": weekend\n";
            break;
        default:
            std::cout << day << ": not a weekend day\n";
        }
    }

    int grade = 2;
    switch (grade)                         // no break: execution falls through
    {
    case 2: std::cout << "case 2\n";
    case 3: std::cout << "case 3 (fell through)\n"; break;
    case 4: std::cout << "case 4\n";
    }

    for (int row = 1; row <= 3; ++row)     // break leaves only the inner loop
    {
        for (int col = 1; col <= 3; ++col)
        {
            if (col > row)
                break;
            std::cout << row * col << ' ';
        }
        std::cout << '\n';
    }
}
