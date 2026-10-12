// Pattern matching with DP (wildcards ? and *, regex . and *) next to the
// standard std::regex.

#include <iostream>
#include <regex>
#include <string>
#include <vector>

bool wildcard(const std::string &s, const std::string &p)
{
    std::vector<std::vector<char>> dp(s.size() + 1, std::vector<char>(p.size() + 1, 0));
    dp[0][0] = 1;
    for (std::size_t j = 1; j <= p.size(); ++j) if (p[j - 1] == '*') dp[0][j] = dp[0][j - 1];
    for (std::size_t i = 1; i <= s.size(); ++i)
        for (std::size_t j = 1; j <= p.size(); ++j)
            if (p[j - 1] == '*') dp[i][j] = dp[i][j - 1] || dp[i - 1][j];          // star matches empty, or one more character
            else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) dp[i][j] = dp[i - 1][j - 1];
    return dp[s.size()][p.size()];
}

bool regex_match_dp(const std::string &s, const std::string &p)                    // '.' any char, 'x*' zero or more x
{
    std::vector<std::vector<char>> dp(s.size() + 1, std::vector<char>(p.size() + 1, 0));
    dp[0][0] = 1;
    for (std::size_t j = 2; j <= p.size(); ++j) if (p[j - 1] == '*') dp[0][j] = dp[0][j - 2];
    for (std::size_t i = 1; i <= s.size(); ++i)
        for (std::size_t j = 1; j <= p.size(); ++j)
        {
            if (p[j - 1] == '*')
                dp[i][j] = (j >= 2 && dp[i][j - 2]) || (j >= 2 && (p[j - 2] == '.' || p[j - 2] == s[i - 1]) && dp[i - 1][j]);
            else if (p[j - 1] == '.' || p[j - 1] == s[i - 1]) dp[i][j] = dp[i - 1][j - 1];
        }
    return dp[s.size()][p.size()];
}

int main()
{
    std::cout << std::boolalpha;
    std::cout << "wildcard adceb / *a*b: " << wildcard("adceb", "*a*b") << ", acdcb / a*c?b: " << wildcard("acdcb", "a*c?b") << '\n';
    std::cout << "regex aa / a*: " << regex_match_dp("aa", "a*") << ", aab / c*a*b: " << regex_match_dp("aab", "c*a*b")
              << ", mississippi / mis*is*p*.: " << regex_match_dp("mississippi", "mis*is*p*.") << '\n';

    std::regex email(R"(([\w.]+)@([\w.]+)\.(com|org))");                           // the standard library regex (ECMAScript syntax)
    std::smatch m;
    std::string text = "write to alice.b@example.com or bob@site.org today";
    for (auto it = std::sregex_iterator(text.begin(), text.end(), email); it != std::sregex_iterator(); ++it)
        std::cout << "found " << (*it)[0] << " user=" << (*it)[1] << " domain=" << (*it)[2] << '\n';
    std::cout << "regex_replace: " << std::regex_replace("2026-10-12", std::regex(R"((\d+)-(\d+)-(\d+))"), "$3/$2/$1") << '\n';
    std::cout << "std::regex is slow and can overflow the stack on long input; prefer hand-written parsing or a library like RE2 for hot paths\n";
}
