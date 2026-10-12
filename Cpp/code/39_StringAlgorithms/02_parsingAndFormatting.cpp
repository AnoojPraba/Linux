// Parsing and formatting text safely: from_chars/to_chars, strtod,
// istringstream, CSV fields with quotes, and stream formatting manipulators.

#include <charconv>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> parse_csv_line(const std::string &line)
{
    std::vector<std::string> fields;
    std::string cur;
    bool quoted = false;
    for (std::size_t i = 0; i < line.size(); ++i)
    {
        char c = line[i];
        if (quoted)
        {
            if (c == '"' && i + 1 < line.size() && line[i + 1] == '"') { cur += '"'; ++i; }   // "" is an escaped quote
            else if (c == '"') quoted = false;
            else cur += c;
        }
        else if (c == '"') quoted = true;
        else if (c == ',') { fields.push_back(cur); cur.clear(); }
        else cur += c;
    }
    fields.push_back(cur);
    return fields;
}

int main()
{
    std::string s = "12345xyz";
    int value = 0;
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);        // no exceptions, no locale, reports where it stopped
    std::cout << "from_chars: " << value << ", stopped at \"" << ptr << "\", error: " << (ec != std::errc{}) << '\n';
    int bad = 0;
    auto r = std::from_chars("abc", "abc" + 3, bad);
    std::cout << "invalid input reports: " << (r.ec == std::errc::invalid_argument ? "invalid_argument" : "other") << '\n';

    char buf[16];
    auto [end, ec2] = std::to_chars(buf, buf + sizeof buf, 255, 16);
    std::cout << "to_chars(255, base 16): " << std::string(buf, end) << '\n';

    std::cout << "csv fields:";
    for (auto &f : parse_csv_line(R"(id,"Smith, John","say ""hi""",,42)")) std::cout << " [" << f << ']';

    std::istringstream in("3.5 42 word");
    double d; int i; std::string w;
    in >> d >> i >> w;
    std::cout << "\nstream extraction: " << d << ' ' << i << ' ' << w << '\n';

    std::cout << std::fixed << std::setprecision(2) << "formatted: [" << std::setw(8) << 3.14159 << "] [" << std::left << std::setw(8) << "left" << "] [" << std::setfill('0') << std::right
              << std::setw(6) << 42 << "] hex " << std::hex << std::showbase << 255 << '\n';
}
