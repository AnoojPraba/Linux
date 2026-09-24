#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

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
    vector<int> numbers = { 1, 2, 3, 4, 5 };
    for (int value : numbers)
    {
        cout << "value = " << value << "\n";
    }

    map<string, int> ages = { { "alice", 30 }, { "bob", 25 } };
    for (const auto &[name, age] : ages)
    {
        cout << name << " is " << age << " years old\n";
    }

    pair<int, int> minMax = { numbers.front(), numbers.back() };
    auto [minValue, maxValue] = minMax;
    cout << "minValue = " << minValue << ", maxValue = " << maxValue << "\n";

    return 0;
}
