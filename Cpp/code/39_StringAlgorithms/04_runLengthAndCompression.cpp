// Run-length encoding and decoding, count-and-say, and in-place style
// character compression.

#include <cctype>
#include <iostream>
#include <string>

std::string rle_encode(const std::string &s)
{
    std::string out;
    for (std::size_t i = 0; i < s.size();)
    {
        std::size_t j = i;
        while (j < s.size() && s[j] == s[i]) ++j;
        out += s[i];
        out += std::to_string(j - i);
        i = j;
    }
    return out;
}

std::string rle_decode(const std::string &s)
{
    std::string out;
    for (std::size_t i = 0; i < s.size();)
    {
        char c = s[i++];
        std::size_t count = 0;
        while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) count = count * 10 + (s[i++] - '0');
        out.append(count, c);
    }
    return out;
}

std::string compress(const std::string &s)                // omit counts of 1: aabcccccaaa -> a2bc5a3
{
    std::string out;
    for (std::size_t i = 0; i < s.size();)
    {
        std::size_t j = i;
        while (j < s.size() && s[j] == s[i]) ++j;
        out += s[i];
        if (j - i > 1) out += std::to_string(j - i);
        i = j;
    }
    return out;
}

std::string count_and_say(int n)
{
    std::string s = "1";
    while (--n > 0)
    {
        std::string next;
        for (std::size_t i = 0; i < s.size();)
        {
            std::size_t j = i;
            while (j < s.size() && s[j] == s[i]) ++j;
            next += std::to_string(j - i) + s[i];             // "say" the run: count then digit
            i = j;
        }
        s = next;
    }
    return s;
}

int main()
{
    std::string e = rle_encode("wwwwaaadexxxxxx");
    std::cout << "encoded: " << e << ", decoded: " << rle_decode(e) << '\n';
    std::cout << "compress aabcccccaaa -> " << compress("aabcccccaaa") << '\n';
    for (int n = 1; n <= 6; ++n) std::cout << "count_and_say(" << n << ") = " << count_and_say(n) << '\n';
    std::cout << "RLE expands data without long runs (abc -> a1b1c1); real compressors (DEFLATE, LZ77) find repeated substrings instead\n";
}
