#include <iostream>
#include <vector>
#include <map>
#include <string>

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates range-based for loops over a vector and a map, plus
 *         structured bindings to unpack a map's key/value pairs and a
 *         function's multi-value return via std::pair.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::vector<int> numbers = { 1, 2, 3, 4, 5 };
    for (int value : numbers)
    {
        std::cout << "value = " << value << "\n";
    }

    std::map<std::string, int> ages = { { "alice", 30 }, { "bob", 25 } };
    for (const auto &[name, age] : ages)
    {
        std::cout << name << " is " << age << " years old\n";
    }

    std::pair<int, int> minMax = { numbers.front(), numbers.back() };
    auto [minValue, maxValue] = minMax;
    std::cout << "minValue = " << minValue << ", maxValue = " << maxValue << "\n";

    return 0;
}
