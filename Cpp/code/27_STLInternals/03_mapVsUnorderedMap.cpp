#include <iostream>
#include <map>
#include <unordered_map>
#include <chrono>

#define ELEMENT_COUNT 100000
#define LOOKUP_COUNT 100000

using namespace std;

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Contrasts std::map (ordered, red-black tree, O(log n) operations)
 *         with std::unordered_map (hash table, average O(1) operations),
 *         printing iteration order for each and a rough timing comparison
 *         of repeated lookups.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    map<int, int> orderedMap;
    unordered_map<int, int> hashMap;

    for (int i = 0; i < ELEMENT_COUNT; i = i + 1)
    {
        orderedMap[i] = i * i;
        hashMap[i] = i * i;
    }

    // std::map always iterates in ascending key order; std::unordered_map's
    // iteration order is unspecified and depends on the hash/bucket layout.
    cout << "map first three keys: ";
    int printed = 0;
    for (const auto &[key, value] : orderedMap)
    {
        if (printed >= 3)
        {
            break;
        }
        cout << key << " ";
        printed = printed + 1;
    }
    cout << "\n";

    auto mapStart = chrono::steady_clock::now();
    long mapSum = 0;
    for (int i = 0; i < LOOKUP_COUNT; i = i + 1)
    {
        mapSum = mapSum + orderedMap[i % ELEMENT_COUNT];
    }
    auto mapEnd = chrono::steady_clock::now();

    auto hashStart = chrono::steady_clock::now();
    long hashSum = 0;
    for (int i = 0; i < LOOKUP_COUNT; i = i + 1)
    {
        hashSum = hashSum + hashMap[i % ELEMENT_COUNT];
    }
    auto hashEnd = chrono::steady_clock::now();

    cout << "map lookups took "
              << chrono::duration_cast<chrono::microseconds>(mapEnd - mapStart).count()
              << " us (sum=" << mapSum << ")\n";
    cout << "unordered_map lookups took "
              << chrono::duration_cast<chrono::microseconds>(hashEnd - hashStart).count()
              << " us (sum=" << hashSum << ")\n";

    return 0;
}
