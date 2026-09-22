#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates sort, find, transform, and accumulate from
 *         <algorithm>/<numeric> on a vector of integers.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::vector<int> numbers = {5, 3, 1, 4, 2};

    std::sort(numbers.begin(), numbers.end());
    std::cout << "sorted: ";
    for (int n : numbers)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";

    auto foundIt = std::find(numbers.begin(), numbers.end(), 4);
    if (foundIt != numbers.end())
    {
        std::cout << "found 4 at index " << (foundIt - numbers.begin()) << "\n";
    }

    std::vector<int> doubled(numbers.size());
    std::transform(numbers.begin(), numbers.end(), doubled.begin(),
                    [](int n) { return n * 2; });
    std::cout << "doubled: ";
    for (int n : doubled)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";

    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    std::cout << "sum = " << sum << "\n";
    return 0;
}
