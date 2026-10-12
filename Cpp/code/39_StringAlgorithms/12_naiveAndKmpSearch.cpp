// Substring search: naive O(nm), KMP O(n + m) with the failure function, and
// the standard library searchers.

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> prefix_function(const std::string &p)
{
    std::vector<int> pi(p.size(), 0);
    for (std::size_t i = 1, k = 0; i < p.size(); ++i)
    {
        while (k > 0 && p[i] != p[k]) k = pi[k - 1];        // fall back along the borders
        if (p[i] == p[k]) ++k;
        pi[i] = static_cast<int>(k);
    }
    return pi;
}

std::vector<int> kmp(const std::string &text, const std::string &pat)
{
    std::vector<int> found, pi = prefix_function(pat);
    for (std::size_t i = 0, k = 0; i < text.size(); ++i)
    {
        while (k > 0 && text[i] != pat[k]) k = pi[k - 1];   // never moves backwards in the text
        if (text[i] == pat[k]) ++k;
        if (k == pat.size()) { found.push_back(static_cast<int>(i + 1 - k)); k = pi[k - 1]; }
    }
    return found;
}

std::vector<int> naive(const std::string &text, const std::string &pat)
{
    std::vector<int> found;
    for (std::size_t i = 0; i + pat.size() <= text.size(); ++i)
        if (text.compare(i, pat.size(), pat) == 0) found.push_back(static_cast<int>(i));
    return found;
}

int main()
{
    std::string text = "abababcabababcabab", pat = "ababc";
    auto show = [](const char *name, const std::vector<int> &v) { std::cout << name << ':'; for (int x : v) std::cout << ' ' << x; std::cout << '\n'; };
    show("naive", naive(text, pat));
    show("KMP  ", kmp(text, pat));
    std::cout << "failure function of " << pat << ':';
    for (int x : prefix_function(pat)) std::cout << ' ' << x;

    auto it = std::search(text.begin(), text.end(), pat.begin(), pat.end());
    std::cout << "\nstd::search first match at " << it - text.begin() << '\n';
    auto bm = std::search(text.begin(), text.end(), std::boyer_moore_searcher(pat.begin(), pat.end()));   // C++17 searchers
    auto bmh = std::search(text.begin(), text.end(), std::boyer_moore_horspool_searcher(pat.begin(), pat.end()));
    std::cout << "boyer_moore_searcher: " << bm - text.begin() << ", horspool: " << bmh - text.begin() << '\n';
    std::cout << "string::find: " << text.find(pat) << " (libstdc++ uses a simple scan: fine for short patterns)\n";
}
