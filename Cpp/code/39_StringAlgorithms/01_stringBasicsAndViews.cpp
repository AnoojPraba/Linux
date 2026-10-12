// std::string essentials, string_view and the common manipulations: find,
// substr, replace, split, join, trim, case conversion.

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

std::vector<std::string_view> split(std::string_view s, char sep)    // views into s: no copies, s must outlive them
{
    std::vector<std::string_view> parts;
    std::size_t start = 0;
    while (true)
    {
        std::size_t end = s.find(sep, start);
        parts.push_back(s.substr(start, end == std::string_view::npos ? end : end - start));
        if (end == std::string_view::npos) return parts;
        start = end + 1;
    }
}

std::string join(const std::vector<std::string_view> &parts, std::string_view sep)
{
    std::string out;
    for (std::size_t i = 0; i < parts.size(); ++i) { if (i) out += sep; out += parts[i]; }
    return out;
}

std::string_view trim(std::string_view s)
{
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.remove_prefix(1);
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.remove_suffix(1);
    return s;
}

int main()
{
    std::string s = "  The quick brown fox, the lazy dog  ";
    std::cout << '[' << trim(s) << "]\n";
    std::string t(trim(s));
    std::cout << "find \"quick\": " << t.find("quick") << ", rfind \"the\": " << t.rfind("the") << ", find \"cat\": " << (t.find("cat") == std::string::npos ? "npos" : "found") << '\n';
    t.replace(t.find("lazy"), 4, "sleepy");
    std::cout << t << '\n';
    std::transform(t.begin(), t.end(), t.begin(), [](unsigned char c) { return std::toupper(c); });
    std::cout << t << '\n';

    auto parts = split("a,b,,c", ',');
    std::cout << "split gives " << parts.size() << " parts (the empty field is kept); join with '|': " << join(parts, "|") << '\n';

    std::istringstream words("alpha  beta\tgamma");          // operator>> skips any whitespace
    std::string w;
    std::cout << "words:";
    while (words >> w) std::cout << " <" << w << '>';
    std::cout << "\nstarts_with-like check: " << std::boolalpha << (t.rfind("THE", 0) == 0) << ", compare(\"apple\",\"banana\") < 0: " << (std::string("apple").compare("banana") < 0) << '\n';
    std::cout << "numeric conversions: " << std::stoi("42abc") << ' ' << std::stod("3.5e2") << ' ' << std::to_string(255) << '\n';
}
