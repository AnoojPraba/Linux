// Validating formatted strings: numbers with a state machine, IPv4 addresses,
// palindromes ignoring punctuation and a simple password policy.

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

bool valid_number(const std::string &s)
{
    std::size_t i = 0;
    int digits = 0;
    bool dot = false;
    if (i < s.size() && (s[i] == '+' || s[i] == '-')) ++i;
    for (; i < s.size() && s[i] != 'e' && s[i] != 'E'; ++i)
    {
        if (std::isdigit(static_cast<unsigned char>(s[i]))) ++digits;
        else if (s[i] == '.' && !dot) dot = true;
        else return false;
    }
    if (!digits) return false;
    if (i < s.size())                                          // exponent: optional sign and at least one digit
    {
        ++i;
        if (i < s.size() && (s[i] == '+' || s[i] == '-')) ++i;
        if (i >= s.size()) return false;
        for (; i < s.size(); ++i) if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

bool valid_ipv4(const std::string &s)
{
    std::istringstream in(s);
    std::string part;
    int parts = 0;
    while (std::getline(in, part, '.'))
    {
        if (part.empty() || part.size() > 3 || (part.size() > 1 && part[0] == '0')) return false;
        if (!std::all_of(part.begin(), part.end(), [](unsigned char c) { return std::isdigit(c); })) return false;
        if (std::stoi(part) > 255) return false;
        ++parts;
    }
    return parts == 4 && std::count(s.begin(), s.end(), '.') == 3;
}

bool palindrome(const std::string &s)
{
    long l = 0, r = static_cast<long>(s.size()) - 1;
    while (l < r)
    {
        while (l < r && !std::isalnum(static_cast<unsigned char>(s[l]))) ++l;
        while (l < r && !std::isalnum(static_cast<unsigned char>(s[r]))) --r;
        if (std::tolower(static_cast<unsigned char>(s[l++])) != std::tolower(static_cast<unsigned char>(s[r--]))) return false;
    }
    return true;
}

bool strong_password(const std::string &p)
{
    auto has = [&](int (*pred)(int)) { return std::any_of(p.begin(), p.end(), [&](unsigned char c) { return pred(c) != 0; }); };
    return p.size() >= 8 && has(std::isupper) && has(std::islower) && has(std::isdigit) && has(std::ispunct);
}

int main()
{
    std::cout << std::boolalpha;
    for (std::string n : {"2", "-0.1", "3.14e10", "1e", ".", "--6", "+.8"}) std::cout << "number " << n << ": " << valid_number(n) << '\n';
    for (std::string ip : {"192.168.1.1", "256.1.1.1", "1.2.3", "01.2.3.4"}) std::cout << "IPv4 " << ip << ": " << valid_ipv4(ip) << '\n';
    std::cout << "palindrome: " << palindrome("A man, a plan, a canal: Panama") << ", " << palindrome("race a car") << '\n';
    std::cout << "password Abcdef1! strong: " << strong_password("Abcdef1!") << ", abcdefgh: " << strong_password("abcdefgh") << '\n';
}
