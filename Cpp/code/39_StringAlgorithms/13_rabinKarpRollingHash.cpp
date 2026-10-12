// Rabin-Karp with a polynomial rolling hash: multi-pattern search and the
// longest repeated substring by binary search on the length.

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using u64 = unsigned long long;
constexpr u64 MOD = 1'000'000'007ULL, BASE = 131;

std::vector<std::size_t> rabin_karp(const std::string &text, const std::string &pat)
{
    std::vector<std::size_t> found;
    if (pat.size() > text.size()) return found;
    u64 hp = 0, ht = 0, pw = 1;
    for (std::size_t i = 0; i < pat.size(); ++i)
    {
        hp = (hp * BASE + static_cast<unsigned char>(pat[i])) % MOD;
        ht = (ht * BASE + static_cast<unsigned char>(text[i])) % MOD;
        if (i) pw = pw * BASE % MOD;                         // BASE^(m-1), used to remove the outgoing character
    }
    for (std::size_t i = 0;; ++i)
    {
        if (hp == ht && text.compare(i, pat.size(), pat) == 0) found.push_back(i);   // verify: equal hashes may collide
        if (i + pat.size() >= text.size()) break;
        ht = (ht + MOD - static_cast<unsigned char>(text[i]) * pw % MOD) % MOD;       // drop text[i]
        ht = (ht * BASE + static_cast<unsigned char>(text[i + pat.size()])) % MOD;    // add the next character
    }
    return found;
}

std::string longest_repeated(const std::string &s)
{
    auto repeated = [&](std::size_t len) -> std::size_t {    // start of a substring of this length that occurs twice, or npos
        std::unordered_map<u64, std::vector<std::size_t>> seen;
        u64 h = 0, pw = 1;
        for (std::size_t i = 0; i + 1 < len; ++i) pw = pw * BASE % MOD;
        for (std::size_t i = 0; i < s.size(); ++i)
        {
            h = (h * BASE + static_cast<unsigned char>(s[i])) % MOD;
            if (i >= len) h = (h + MOD - static_cast<unsigned char>(s[i - len]) * pw % MOD * BASE % MOD) % MOD;
            if (i + 1 >= len)
            {
                std::size_t start = i + 1 - len;
                for (std::size_t other : seen[h]) if (s.compare(other, len, s, start, len) == 0) return start;
                seen[h].push_back(start);
            }
        }
        return std::string::npos;
    };
    std::size_t lo = 1, hi = s.size() - 1, best = 0, at = 0;
    while (lo <= hi)                                         // "a repeat of length L exists" is monotone in L
    {
        std::size_t mid = (lo + hi) / 2, p = repeated(mid);
        if (p != std::string::npos) { best = mid; at = p; lo = mid + 1; } else hi = mid - 1;
    }
    return s.substr(at, best);
}

int main()
{
    std::string text = "abracadabra abracadabra";
    std::cout << "\"abra\" found at:";
    for (auto p : rabin_karp(text, "abra")) std::cout << ' ' << p;
    std::cout << "\nlongest repeated substring of abcxabcdabcxabc: " << longest_repeated("abcxabcdabcxabc") << '\n';
    std::cout << "use two moduli or a 64-bit modulus to make collisions negligible; always verify on a hash match\n";
}
