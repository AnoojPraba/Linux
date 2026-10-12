// Anagram problems: valid anagram, group anagrams, find all anagram positions
// with a window, and permutation-in-string.

#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

bool is_anagram(const std::string &a, const std::string &b)
{
    std::array<int, 26> cnt{};
    for (char c : a) ++cnt[c - 'a'];
    for (char c : b) --cnt[c - 'a'];
    return a.size() == b.size() && std::all_of(cnt.begin(), cnt.end(), [](int x) { return x == 0; });
}

std::vector<int> find_anagrams(const std::string &s, const std::string &p)
{
    std::vector<int> result;
    if (s.size() < p.size()) return result;
    std::array<int, 26> need{}, window{};
    for (char c : p) ++need[c - 'a'];
    for (std::size_t i = 0; i < s.size(); ++i)
    {
        ++window[s[i] - 'a'];
        if (i >= p.size()) --window[s[i - p.size()] - 'a'];               // keep a window of exactly |p| characters
        if (i + 1 >= p.size() && window == need) result.push_back(static_cast<int>(i + 1 - p.size()));
    }
    return result;
}

int main()
{
    std::cout << std::boolalpha << "listen / silent: " << is_anagram("listen", "silent") << ", rat / car: " << is_anagram("rat", "car") << '\n';

    std::map<std::string, std::vector<std::string>> groups;                // sorted letters are the group key
    for (std::string w : {"eat", "tea", "tan", "ate", "nat", "bat"})
    {
        std::string key = w;
        std::sort(key.begin(), key.end());
        groups[key].push_back(w);
    }
    for (auto &[key, words] : groups)
    {
        std::cout << "  group " << key << ":";
        for (auto &w : words) std::cout << ' ' << w;
        std::cout << '\n';
    }
    std::cout << "anagrams of abc in cbaebabacd start at:";
    for (int i : find_anagrams("cbaebabacd", "abc")) std::cout << ' ' << i;
    std::cout << "\npermutation of ab in eidbaooo: " << !find_anagrams("eidbaooo", "ab").empty() << '\n';
}
