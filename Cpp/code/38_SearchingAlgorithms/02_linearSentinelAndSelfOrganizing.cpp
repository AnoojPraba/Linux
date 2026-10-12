// Linear search and its improvements: std::find family, sentinel search, move-
// to-front and transpose heuristics.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

long cost;

template <typename Cont, typename T>
void move_to_front_find(Cont &c, const T &key)             // recently used items migrate to the front
{
    for (auto it = c.begin(); it != c.end(); ++it)
    {
        ++cost;
        if (*it == key) { c.splice(c.begin(), c, it); return; }   // list::splice moves the node in O(1)
    }
}

int main()
{
    std::vector<int> v{5, 9, 2, 7, 4};
    std::cout << "find 7: index " << std::find(v.begin(), v.end(), 7) - v.begin() << ", find_if > 8: " << *std::find_if(v.begin(), v.end(), [](int x) { return x > 8; })
              << ", count_if even: " << std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }) << '\n';

    std::vector<int> s{5, 9, 2, 7, 4};                      // sentinel: one comparison per element instead of two
    s.push_back(7);                                         // the key itself guarantees the loop stops
    std::size_t i = 0;
    while (s[i] != 7) ++i;
    std::cout << "sentinel search for 7 stops at index " << i << (i == s.size() - 1 ? " (not found)" : " (found)") << '\n';

    std::list<int> plain{1, 2, 3, 4, 5, 6, 7, 8}, mtf = plain;
    const int queries[] = {7, 7, 3, 7, 7, 3, 7, 7, 7, 3};
    long plain_cost = 0;
    for (int q : queries) plain_cost += std::distance(plain.begin(), std::find(plain.begin(), plain.end(), q)) + 1;
    cost = 0;
    for (int q : queries) move_to_front_find(mtf, q);
    std::cout << "10 skewed queries: plain scan " << plain_cost << " comparisons, move-to-front " << cost << ", list is now";
    for (int x : mtf) std::cout << ' ' << x;
    std::cout << "\nlinear search needs no ordering and works on forward-only containers; for repeated lookups sort first, or use a hash table\n";
}
