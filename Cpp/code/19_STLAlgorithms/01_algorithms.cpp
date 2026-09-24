#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

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
    vector<int> numbers = {5, 3, 1, 4, 2};

    sort(numbers.begin(), numbers.end());
    cout << "sorted: ";
    for (int n : numbers)
    {
        cout << n << " ";
    }
    cout << "\n";

    auto foundIt = find(numbers.begin(), numbers.end(), 4);
    if (foundIt != numbers.end())
    {
        cout << "found 4 at index " << (foundIt - numbers.begin()) << "\n";
    }

    vector<int> doubled(numbers.size());
    transform(numbers.begin(), numbers.end(), doubled.begin(),
                    [](int n) { return n * 2; });
    cout << "doubled: ";
    for (int n : doubled)
    {
        cout << n << " ";
    }
    cout << "\n";

    int sum = accumulate(numbers.begin(), numbers.end(), 0);
    cout << "sum = " << sum << "\n";
    return 0;
}
