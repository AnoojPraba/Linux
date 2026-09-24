#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <string>

using namespace std;

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
    vector<int> numbers = {5, 3, 1, 4};
    numbers.push_back(2);
    cout << "vector: ";
    for (int n : numbers)
    {
        cout << n << " ";
    }
    cout << "\n";

    map<string, int> ages;
    ages["Alice"] = 30;
    ages["Bob"] = 25;
    cout << "map:\n";
    for (const auto &entry : ages)
    {
        cout << "  " << entry.first << " -> " << entry.second << "\n";
    }

    set<int> uniqueValues = {4, 2, 4, 1, 2};
    cout << "set: ";
    for (int value : uniqueValues)
    {
        cout << value << " ";
    }
    cout << "\n";

    unordered_map<string, double> prices;
    prices["apple"] = 1.5;
    prices["bread"] = 2.75;
    auto found = prices.find("apple");
    if (found != prices.end())
    {
        cout << "unordered_map lookup: apple costs " << found->second << "\n";
    }
    return 0;
}
