// Suffix array by prefix doubling (O(n log^2 n)), Kasai's LCP array, longest
// repeated substring, distinct substrings and pattern search by binary search.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<int> suffix_array(const std::string &s)
{
    int n = static_cast<int>(s.size());
    std::vector<int> sa(n), rank(s.begin(), s.end()), tmp(n);
    std::iota(sa.begin(), sa.end(), 0);
    for (int k = 1;; k <<= 1)
    {
        auto cmp = [&](int a, int b) {                       // compare by (rank of first k, rank of the next k)
            if (rank[a] != rank[b]) return rank[a] < rank[b];
            int ra = a + k < n ? rank[a + k] : -1, rb = b + k < n ? rank[b + k] : -1;
            return ra < rb;
        };
        std::sort(sa.begin(), sa.end(), cmp);
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; ++i) tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
        rank = tmp;
        if (rank[sa[n - 1]] == n - 1) break;                 // all ranks distinct: sorted
    }
    return sa;
}

std::vector<int> kasai(const std::string &s, const std::vector<int> &sa)
{
    int n = static_cast<int>(s.size());
    std::vector<int> rank(n), lcp(n, 0);
    for (int i = 0; i < n; ++i) rank[sa[i]] = i;
    for (int i = 0, h = 0; i < n; ++i)
    {
        if (rank[i] > 0)
        {
            int j = sa[rank[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) ++h;
            lcp[rank[i]] = h;
            if (h > 0) --h;                                  // the next suffix loses at most one matching character
        }
        else h = 0;
    }
    return lcp;
}

int main()
{
    std::string s = "banana";
    auto sa = suffix_array(s);
    auto lcp = kasai(s, sa);
    std::cout << "suffix array of " << s << ":\n";
    for (std::size_t i = 0; i < sa.size(); ++i) std::cout << "  " << sa[i] << ' ' << s.substr(sa[i]) << "  lcp=" << lcp[i] << '\n';

    int best = *std::max_element(lcp.begin(), lcp.end());
    int at = sa[std::max_element(lcp.begin(), lcp.end()) - lcp.begin()];
    long distinct = static_cast<long>(s.size()) * (s.size() + 1) / 2 - std::accumulate(lcp.begin(), lcp.end(), 0L);
    std::cout << "longest repeated substring: " << s.substr(at, best) << ", distinct substrings: " << distinct << '\n';

    std::string pat = "ana";                                  // pattern search = binary search over the sorted suffixes
    auto lo = std::lower_bound(sa.begin(), sa.end(), pat, [&](int i, const std::string &p) { return s.compare(i, p.size(), p) < 0; });
    auto hi = std::upper_bound(sa.begin(), sa.end(), pat, [&](const std::string &p, int i) { return s.compare(i, p.size(), p) > 0; });
    std::cout << "\"" << pat << "\" occurs " << hi - lo << " times, at:";
    for (auto it = lo; it != hi; ++it) std::cout << ' ' << *it;
    std::cout << '\n';
}
