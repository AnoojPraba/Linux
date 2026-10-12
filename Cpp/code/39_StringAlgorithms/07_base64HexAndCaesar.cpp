// Text encodings: hexadecimal, Base64 with padding, URL-style percent
// encoding, Caesar and Vigenere ciphers.

#include <cctype>
#include <iostream>
#include <string>

std::string to_hex(const std::string &s)
{
    static const char *digits = "0123456789abcdef";
    std::string out;
    for (unsigned char c : s) { out += digits[c >> 4]; out += digits[c & 15]; }
    return out;
}

std::string base64_encode(const std::string &in)
{
    static const std::string table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    for (std::size_t i = 0; i < in.size(); i += 3)
    {
        unsigned v = static_cast<unsigned char>(in[i]) << 16;                       // 3 bytes = 24 bits = four 6-bit groups
        if (i + 1 < in.size()) v |= static_cast<unsigned char>(in[i + 1]) << 8;
        if (i + 2 < in.size()) v |= static_cast<unsigned char>(in[i + 2]);
        out += table[v >> 18 & 63];
        out += table[v >> 12 & 63];
        out += i + 1 < in.size() ? table[v >> 6 & 63] : '=';
        out += i + 2 < in.size() ? table[v & 63] : '=';
    }
    return out;
}

std::string base64_decode(const std::string &in)
{
    static const std::string table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    unsigned acc = 0;
    int bits = 0;
    for (char c : in)
    {
        if (c == '=') break;
        acc = acc << 6 | static_cast<unsigned>(table.find(c));
        bits += 6;
        if (bits >= 8) { bits -= 8; out += static_cast<char>(acc >> bits & 255); }
    }
    return out;
}

std::string percent_encode(const std::string &s)
{
    std::string out;
    for (unsigned char c : s)
    {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') out += static_cast<char>(c);
        else { out += '%'; out += "0123456789ABCDEF"[c >> 4]; out += "0123456789ABCDEF"[c & 15]; }
    }
    return out;
}

std::string caesar(std::string s, int shift)
{
    for (char &c : s)
        if (std::isalpha(static_cast<unsigned char>(c)))
        {
            char base = std::isupper(static_cast<unsigned char>(c)) ? 'A' : 'a';
            c = static_cast<char>(base + (c - base + shift % 26 + 26) % 26);
        }
    return s;
}

std::string vigenere(std::string s, const std::string &key, bool decrypt)
{
    std::size_t k = 0;
    for (char &c : s)
        if (std::isalpha(static_cast<unsigned char>(c)))
        {
            int shift = std::toupper(static_cast<unsigned char>(key[k++ % key.size()])) - 'A';
            c = caesar(std::string(1, c), decrypt ? -shift : shift)[0];
        }
    return s;
}

int main()
{
    std::cout << "hex of \"Hi!\": " << to_hex("Hi!") << '\n';
    std::string enc = base64_encode("Many hands make light work.");
    std::cout << "base64: " << enc << "\ndecoded: " << base64_decode(enc) << "\npadding: Ma -> " << base64_encode("Ma") << ", M -> " << base64_encode("M") << '\n';
    std::cout << "percent encoding: " << percent_encode("a b&c=d/é") << '\n';
    std::string c = caesar("Hello, World!", 3);
    std::cout << "caesar +3: " << c << " -> " << caesar(c, -3) << '\n';
    std::string v = vigenere("ATTACKATDAWN", "LEMON", false);
    std::cout << "vigenere: " << v << " -> " << vigenere(v, "LEMON", true) << '\n';
}
