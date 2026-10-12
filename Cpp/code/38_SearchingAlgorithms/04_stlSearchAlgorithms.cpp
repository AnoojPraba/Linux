// The STL searching toolbox: find, adjacent_find, search, find_end,
// find_first_of, mismatch, includes and set/map lookups.

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

int main()
{
    std::vector<int> v{3, 8, 8, 1, 4, 1, 5, 9, 2, 6};
    std::cout << "adjacent_find (first equal neighbours): index " << std::adjacent_find(v.begin(), v.end()) - v.begin() << '\n';

    std::vector<int> pat{1, 4, 1};
    std::cout << "search {1,4,1} as a subsequence: index " << std::search(v.begin(), v.end(), pat.begin(), pat.end()) - v.begin() << '\n';
    std::cout << "find_end (last occurrence of {1}): index " << std::find_end(v.begin(), v.end(), pat.begin() + 2, pat.end()) - v.begin() << '\n';

    std::vector<int> wanted{9, 5};
    std::cout << "find_first_of {9,5}: index " << std::find_first_of(v.begin(), v.end(), wanted.begin(), wanted.end()) - v.begin() << '\n';

    std::string a = "interview", b = "internet";
    auto [pa, pb] = std::mismatch(a.begin(), a.end(), b.begin(), b.end());
    std::cout << "mismatch: common prefix \"" << std::string(a.begin(), pa) << "\", then '" << *pa << "' vs '" << *pb << "'\n";

    std::vector<int> big{1, 2, 3, 4, 5, 6, 7, 8}, small{2, 5, 7};
    std::cout << "includes (sorted subset): " << std::boolalpha << std::includes(big.begin(), big.end(), small.begin(), small.end()) << '\n';

    std::set<int> s{10, 20, 30, 40};                         // ordered containers search in O(log n) with member functions
    std::cout << "set lower_bound(25) = " << *s.lower_bound(25) << ", contains 30: " << s.contains(30) << '\n';
    std::map<std::string, int> m{{"apple", 3}, {"pear", 5}};
    if (auto it = m.find("pear"); it != m.end()) std::cout << "map find pear -> " << it->second << '\n';
    std::cout << "min_element " << *std::min_element(v.begin(), v.end()) << ", max_element " << *std::max_element(v.begin(), v.end())
              << ", any_of > 8: " << std::any_of(v.begin(), v.end(), [](int x) { return x > 8; }) << ", none_of < 0: " << std::none_of(v.begin(), v.end(), [](int x) { return x < 0; }) << '\n';
}
