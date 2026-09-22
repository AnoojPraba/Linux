#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <string>

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates basic usage of vector, map, set, and unordered_map:
 *         insertion, iteration, and lookup.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::vector<int> numbers = {5, 3, 1, 4};
    numbers.push_back(2);
    std::cout << "vector: ";
    for (int n : numbers)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";

    std::map<std::string, int> ages;
    ages["Alice"] = 30;
    ages["Bob"] = 25;
    std::cout << "map:\n";
    for (const auto &entry : ages)
    {
        std::cout << "  " << entry.first << " -> " << entry.second << "\n";
    }

    std::set<int> uniqueValues = {4, 2, 4, 1, 2};
    std::cout << "set: ";
    for (int value : uniqueValues)
    {
        std::cout << value << " ";
    }
    std::cout << "\n";

    std::unordered_map<std::string, double> prices;
    prices["apple"] = 1.5;
    prices["bread"] = 2.75;
    auto found = prices.find("apple");
    if (found != prices.end())
    {
        std::cout << "unordered_map lookup: apple costs " << found->second << "\n";
    }
    return 0;
}
