// String DP: edit distance (with the edit script), LCS, longest common
// substring, longest palindromic subsequence and the shortest common
// supersequence length.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using Table = std::vector<std::vector<int>>;

int edit_distance(const std::string &a, const std::string &b, std::vector<std::string> *script = nullptr)
{
    Table dp(a.size() + 1, std::vector<int>(b.size() + 1));
    for (std::size_t i = 0; i <= a.size(); ++i) dp[i][0] = static_cast<int>(i);
    for (std::size_t j = 0; j <= b.size(); ++j) dp[0][j] = static_cast<int>(j);
    for (std::size_t i = 1; i <= a.size(); ++i)
        for (std::size_t j = 1; j <= b.size(); ++j)
            dp[i][j] = std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + (a[i - 1] != b[j - 1])});
    if (script)                                              // walk back to list one optimal sequence of edits
        for (std::size_t i = a.size(), j = b.size(); i > 0 || j > 0;)
        {
            if (i && j && dp[i][j] == dp[i - 1][j - 1] + (a[i - 1] != b[j - 1])) { if (a[i - 1] != b[j - 1]) script->push_back(std::string("replace ") + a[i - 1] + " with " + b[j - 1]); --i; --j; }
            else if (i && dp[i][j] == dp[i - 1][j] + 1) { script->push_back(std::string("delete ") + a[i - 1]); --i; }
            else { script->push_back(std::string("insert ") + b[j - 1]); --j; }
        }
    return dp[a.size()][b.size()];
}

std::string lcs(const std::string &a, const std::string &b)
{
    Table dp(a.size() + 1, std::vector<int>(b.size() + 1, 0));
    for (std::size_t i = 1; i <= a.size(); ++i)
        for (std::size_t j = 1; j <= b.size(); ++j)
            dp[i][j] = a[i - 1] == b[j - 1] ? dp[i - 1][j - 1] + 1 : std::max(dp[i - 1][j], dp[i][j - 1]);
    std::string out;
    for (std::size_t i = a.size(), j = b.size(); i > 0 && j > 0;)
        if (a[i - 1] == b[j - 1]) { out += a[i - 1]; --i; --j; }
        else if (dp[i - 1][j] >= dp[i][j - 1]) --i; else --j;
    return {out.rbegin(), out.rend()};
}

std::string longest_common_substring(const std::string &a, const std::string &b)
{
    Table dp(a.size() + 1, std::vector<int>(b.size() + 1, 0));
    int best = 0;
    std::size_t end = 0;
    for (std::size_t i = 1; i <= a.size(); ++i)
        for (std::size_t j = 1; j <= b.size(); ++j)
            if (a[i - 1] == b[j - 1]) { dp[i][j] = dp[i - 1][j - 1] + 1; if (dp[i][j] > best) { best = dp[i][j]; end = i; } }
    return a.substr(end - best, best);
}

int main()
{
    std::vector<std::string> script;
    std::cout << "edit distance kitten -> sitting = " << edit_distance("kitten", "sitting", &script) << ", script (from the end):";
    for (auto &s : script) std::cout << " [" << s << ']';
    std::cout << "\nLCS(AGGTAB, GXTXAYB) = " << lcs("AGGTAB", "GXTXAYB") << '\n';
    std::cout << "longest common substring = " << longest_common_substring("zxabcdezy", "yzabcdezx") << '\n';
    std::string s = "character", r(s.rbegin(), s.rend());
    std::cout << "longest palindromic subsequence of " << s << " = " << lcs(s, r) << " (LCS with its reverse)\n";
    std::cout << "shortest common supersequence length of AGGTAB and GXTXAYB = " << 6 + 7 - static_cast<int>(lcs("AGGTAB", "GXTXAYB").size()) << " (|a| + |b| - |LCS|)\n";
}
