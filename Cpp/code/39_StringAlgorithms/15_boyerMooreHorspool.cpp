// Boyer-Moore-Horspool with a bad-character shift table, counting comparisons
// against the naive scan.

#include <array>
#include <iostream>
#include <string>

long horspool(const std::string &text, const std::string &pat, long &comparisons)
{
    std::array<std::size_t, 256> shift;
    shift.fill(pat.size());                                  // a character not in the pattern skips the whole pattern
    for (std::size_t i = 0; i + 1 < pat.size(); ++i) shift[static_cast<unsigned char>(pat[i])] = pat.size() - 1 - i;
    comparisons = 0;
    for (std::size_t pos = 0; pos + pat.size() <= text.size();)
    {
        std::size_t j = pat.size();
        while (j > 0 && (++comparisons, text[pos + j - 1] == pat[j - 1])) --j;        // compare right to left
        if (j == 0) return static_cast<long>(pos);
        pos += shift[static_cast<unsigned char>(text[pos + pat.size() - 1])];
    }
    return -1;
}

int main()
{
    std::string text = "the quick brown fox jumps over the lazy dog and keeps running far away";
    for (std::string pat : {"lazy", "running", "zebra"})
    {
        long cmp;
        long at = horspool(text, pat, cmp);
        long naive = 0;
        for (std::size_t i = 0; i + pat.size() <= text.size(); ++i)
            for (std::size_t j = 0; j < pat.size(); ++j) { ++naive; if (text[i + j] != pat[j]) break; }
        std::cout << pat << " -> " << at << " with " << cmp << " comparisons (naive scan: about " << naive << ")\n";
    }
}
