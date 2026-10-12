// Z-algorithm and the prefix function: pattern search, borders, smallest
// period and shortest palindrome by prepending.

#include <iostream>
#include <string>
#include <vector>

std::vector<int> z_function(const std::string &s)
{
    int n = static_cast<int>(s.size());
    std::vector<int> z(n, 0);
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; ++i)
    {
        if (i < r) z[i] = std::min(r - i, z[i - l]);        // reuse the Z-box [l, r)
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] > r) { l = i; r = i + z[i]; }
    }
    return z;
}

std::vector<int> prefix_function(const std::string &s)
{
    std::vector<int> pi(s.size(), 0);
    for (std::size_t i = 1; i < s.size(); ++i)
    {
        int k = pi[i - 1];
        while (k > 0 && s[i] != s[k]) k = pi[k - 1];
        if (s[i] == s[k]) ++k;
        pi[i] = k;
    }
    return pi;
}

int main()
{
    std::string text = "abxabcabcaby", pat = "abcaby";
    auto z = z_function(pat + "#" + text);
    std::cout << "pattern found at:";
    for (std::size_t i = pat.size() + 1; i < z.size(); ++i)
        if (z[i] == static_cast<int>(pat.size())) std::cout << ' ' << i - pat.size() - 1;

    std::string s = "abcabcabc";
    auto pi = prefix_function(s);
    int n = static_cast<int>(s.size()), period = n - pi.back();
    std::cout << "\n" << s << ": smallest period " << period << ", repetition of a shorter block: " << std::boolalpha << (n % period == 0 && period < n) << ", borders:";
    for (int k = pi.back(); k > 0; k = pi[k - 1]) std::cout << " \"" << s.substr(0, k) << '"';

    std::string t = "aacecaaa";                              // shortest palindrome by adding characters in front
    std::string r(t.rbegin(), t.rend());
    int keep = prefix_function(t + "#" + r).back();         // length of the longest palindromic prefix
    std::cout << "\nshortest palindrome for " << t << ": " << r.substr(0, t.size() - keep) + t << '\n';
}
