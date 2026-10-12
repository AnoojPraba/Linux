// Sliding-window string problems: minimum window substring, longest substring
// with at most k distinct characters and without repeats.

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <unordered_map>

std::string min_window(const std::string &s, const std::string &t)
{
    std::array<int, 128> need{};
    for (char c : t) ++need[static_cast<unsigned char>(c)];
    int missing = static_cast<int>(t.size());
    std::size_t best_start = 0, best_len = s.size() + 1;
    for (std::size_t l = 0, r = 0; r < s.size(); ++r)
    {
        if (need[static_cast<unsigned char>(s[r])]-- > 0) --missing;      // a still-needed character entered
        while (missing == 0)                                              // valid: shrink while it stays valid
        {
            if (r - l + 1 < best_len) { best_len = r - l + 1; best_start = l; }
            if (++need[static_cast<unsigned char>(s[l++])] > 0) ++missing;
        }
    }
    return best_len > s.size() ? "" : s.substr(best_start, best_len);
}

std::size_t longest_k_distinct(const std::string &s, std::size_t k)
{
    std::unordered_map<char, int> count;
    std::size_t best = 0;
    for (std::size_t l = 0, r = 0; r < s.size(); ++r)
    {
        ++count[s[r]];
        while (count.size() > k) { if (--count[s[l]] == 0) count.erase(s[l]); ++l; }
        best = std::max(best, r - l + 1);
    }
    return best;
}

std::size_t longest_unique(const std::string &s)
{
    std::array<long, 256> last;
    last.fill(-1);
    std::size_t best = 0;
    long left = 0;
    for (long r = 0; r < static_cast<long>(s.size()); ++r)
    {
        left = std::max(left, last[static_cast<unsigned char>(s[r])] + 1);       // jump past the previous copy
        last[static_cast<unsigned char>(s[r])] = r;
        best = std::max(best, static_cast<std::size_t>(r - left + 1));
    }
    return best;
}

int main()
{
    std::cout << "minimum window of ADOBECODEBANC covering ABC: " << min_window("ADOBECODEBANC", "ABC") << '\n';
    std::cout << "longest substring with <= 2 distinct characters in eceba: " << longest_k_distinct("eceba", 2) << '\n';
    std::cout << "longest substring without repeats in abcabcbb: " << longest_unique("abcabcbb") << ", in pwwkew: " << longest_unique("pwwkew") << '\n';
    std::cout << "each pointer only moves forward, so all of these are O(n)\n";
}
