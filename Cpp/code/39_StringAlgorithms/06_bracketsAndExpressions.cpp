// Stack problems on strings: balanced brackets, longest valid parentheses,
// minimum additions, generating parentheses and expression evaluation.

#include <cctype>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

bool balanced(const std::string &s)
{
    std::stack<char> st;
    for (char c : s)
    {
        if (c == '(' || c == '[' || c == '{') st.push(c);
        else if (c == ')' || c == ']' || c == '}')
        {
            if (st.empty()) return false;
            char o = st.top(); st.pop();
            if ((c == ')' && o != '(') || (c == ']' && o != '[') || (c == '}' && o != '{')) return false;
        }
    }
    return st.empty();
}

int longest_valid(const std::string &s)
{
    std::stack<int> st;
    st.push(-1);                                             // the index just before the current valid run
    int best = 0;
    for (int i = 0; i < static_cast<int>(s.size()); ++i)
    {
        if (s[i] == '(') st.push(i);
        else
        {
            st.pop();
            if (st.empty()) st.push(i);                       // unmatched ')': a new base
            else best = std::max(best, i - st.top());
        }
    }
    return best;
}

void generate(int open, int close, std::string &cur, std::vector<std::string> &out)
{
    if (open == 0 && close == 0) { out.push_back(cur); return; }
    if (open > 0) { cur += '('; generate(open - 1, close + 1, cur, out); cur.pop_back(); }
    if (close > 0) { cur += ')'; generate(open, close - 1, cur, out); cur.pop_back(); }   // only close what is open
}

struct Parser                                                // recursive descent: expr -> term -> factor
{
    const std::string &s;
    std::size_t i = 0;
    void skip() { while (i < s.size() && s[i] == ' ') ++i; }
    long factor()
    {
        skip();
        if (s[i] == '(') { ++i; long v = expr(); skip(); ++i; return v; }
        if (s[i] == '-') { ++i; return -factor(); }
        long v = 0;
        while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) v = v * 10 + (s[i++] - '0');
        return v;
    }
    long term()
    {
        long v = factor();
        for (skip(); i < s.size() && (s[i] == '*' || s[i] == '/'); skip()) { char op = s[i++]; long r = factor(); v = op == '*' ? v * r : v / r; }
        return v;
    }
    long expr()
    {
        long v = term();
        for (skip(); i < s.size() && (s[i] == '+' || s[i] == '-'); skip()) { char op = s[i++]; long r = term(); v = op == '+' ? v + r : v - r; }
        return v;
    }
};

int main()
{
    std::cout << std::boolalpha;
    for (std::string s : {"{[()]}", "([)]", "(("}) std::cout << "balanced " << s << ": " << balanced(s) << '\n';
    std::cout << "longest valid parentheses in )()()): " << longest_valid(")()())") << '\n';
    std::vector<std::string> all;
    std::string cur;
    generate(3, 0, cur, all);
    std::cout << "all valid arrangements of 3 pairs (" << all.size() << " = Catalan(3)):";
    for (auto &a : all) std::cout << ' ' << a;
    std::string expr = "2 + 3 * (4 - 1) / 3 - -2";
    Parser p{expr};
    std::cout << "\n" << expr << " = " << p.expr() << '\n';
}
