#include <iostream>
#include <vector>
#include <algorithm>

#define THRESHOLD 5

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates lambda syntax, captures by value and by reference, and
 *         using lambdas with STL algorithms.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::vector<int> numbers = {1, 6, 3, 8, 2, 9};

    auto isAboveThreshold = [](int n) { return n > THRESHOLD; };
    int countAbove = static_cast<int>(
        std::count_if(numbers.begin(), numbers.end(), isAboveThreshold));
    std::cout << "count above " << THRESHOLD << ": " << countAbove << "\n";

    int total = 0;
    std::for_each(numbers.begin(), numbers.end(), [&total](int n) { total += n; });
    std::cout << "sum via capture-by-reference lambda: " << total << "\n";

    int multiplier = 10;
    auto scale = [multiplier](int n) { return n * multiplier; };
    std::cout << "scale(3) with capture-by-value multiplier: " << scale(3) << "\n";

    std::sort(numbers.begin(), numbers.end(),
              [](int a, int b) { return a > b; });
    std::cout << "sorted descending: ";
    for (int n : numbers)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";
    return 0;
}
