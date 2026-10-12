// Generating strings: permutations (with duplicates), subsequences,
// combinations, next_permutation and the kth permutation by factorial
// numbering.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

void permute(std::string s, std::size_t k, std::vector<std::string> &out)   // distinct permutations of a string with duplicates
{
    if (k == s.size()) { out.push_back(s); return; }
    for (std::size_t i = k; i < s.size(); ++i)
    {
        if (std::find(s.begin() + k, s.begin() + i, s[i]) != s.begin() + i) continue;   // this character was already tried here
        std::swap(s[k], s[i]);
        permute(s, k + 1, out);                                                       // s is passed by value: backtracking is implicit
    }
}

std::string kth_permutation(int n, int k)                    // 1-based k, by the factorial number system
{
    std::vector<int> fact(n + 1, 1);
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
    std::string digits(n, '0'), result;
    std::iota(digits.begin(), digits.end(), '1');
    --k;
    for (int i = n; i >= 1; --i)
    {
        int idx = k / fact[i - 1];
        result += digits[idx];
        digits.erase(digits.begin() + idx);
        k %= fact[i - 1];
    }
    return result;
}

void combinations(int n, int k, int start, std::vector<int> &cur, std::vector<std::vector<int>> &out)
{
    if (static_cast<int>(cur.size()) == k) { out.push_back(cur); return; }
    for (int i = start; i <= n; ++i) { cur.push_back(i); combinations(n, k, i + 1, cur, out); cur.pop_back(); }
}

int main()
{
    std::vector<std::string> p;
    permute("aab", 0, p);
    std::cout << "distinct permutations of aab:";
    for (auto &s : p) std::cout << ' ' << s;

    std::string t = "abc";
    std::cout << "\nsubsequences of abc:";
    for (int mask = 0; mask < (1 << t.size()); ++mask)
    {
        std::string sub;
        for (std::size_t i = 0; i < t.size(); ++i) if (mask >> i & 1) sub += t[i];
        std::cout << " \"" << sub << '"';
    }

    std::string n = "1243";
    std::next_permutation(n.begin(), n.end());
    std::cout << "\nnext_permutation of 1243: " << n << ", all permutations of abc via do-while:";
    std::string a = "abc";
    do std::cout << ' ' << a; while (std::next_permutation(a.begin(), a.end()));

    std::cout << "\n3rd permutation of 1..4: " << kth_permutation(4, 3) << ", 9th: " << kth_permutation(4, 9);
    std::vector<std::vector<int>> c;
    std::vector<int> cur;
    combinations(4, 2, 1, cur, c);
    std::cout << "\nC(4,2) = " << c.size() << ':';
    for (auto &v : c) std::cout << " {" << v[0] << ',' << v[1] << '}';
    std::cout << '\n';
}
