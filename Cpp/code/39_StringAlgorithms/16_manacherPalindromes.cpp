// Palindromic substrings: expand-around-centre O(n^2) and Manacher O(n);
// longest, count and all centres.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::string expand_longest(const std::string &s)
{
    std::size_t best_l = 0, best_len = 0;
    for (std::size_t c = 0; c < s.size(); ++c)
        for (int parity = 0; parity < 2; ++parity)           // odd and even length centres
        {
            long l = static_cast<long>(c), r = static_cast<long>(c) + parity;
            while (l >= 0 && r < static_cast<long>(s.size()) && s[l] == s[r]) { --l; ++r; }
            if (static_cast<std::size_t>(r - l - 1) > best_len) { best_len = r - l - 1; best_l = static_cast<std::size_t>(l + 1); }
        }
    return s.substr(best_l, best_len);
}

int main()
{
    std::string s = "babcbabcbaccba";
    std::string t = "^";                                     // ^#b#a#...#$ makes odd and even palindromes share a centre
    for (char c : s) { t += '#'; t += c; }
    t += "#$";
    std::vector<int> p(t.size(), 0);
    int centre = 0, right = 0;
    for (int i = 1; i + 1 < static_cast<int>(t.size()); ++i)
    {
        if (i < right) p[i] = std::min(right - i, p[2 * centre - i]);   // mirror inside the current palindrome
        while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) ++p[i];
        if (i + p[i] > right) { centre = i; right = i + p[i]; }
    }
    int best = 0, at = 0;
    long count = 0;
    for (int i = 1; i + 1 < static_cast<int>(t.size()); ++i)
    {
        if (p[i] > best) { best = p[i]; at = i; }
        count += (p[i] + 1) / 2;
    }
    std::cout << "Manacher: longest palindrome " << s.substr((at - best) / 2, best) << " (length " << best << "), " << count << " palindromic substrings\n";
    std::cout << "expand around centre agrees: " << expand_longest(s) << '\n';
}
