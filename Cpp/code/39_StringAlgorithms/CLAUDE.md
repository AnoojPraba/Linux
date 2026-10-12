# 39_StringAlgorithms

String algorithms in modern C++: std::string/string_view manipulation, substring search (KMP, Z, Rabin-Karp, Boyer-Moore-Horspool, Aho-Corasick), palindromes, suffix arrays, string DP and pattern matching.

## Files
- `01_stringBasicsAndViews.cpp` - std::string essentials, string_view and the common manipulations: find, substr, replace, split, join, trim, case conversion.
- `02_parsingAndFormatting.cpp` - Parsing and formatting text safely: from_chars/to_chars, strtod, istringstream, CSV fields with quotes, and stream formatting manipulators.
- `03_validationNumbersIpPalindrome.cpp` - Validating formatted strings: numbers with a state machine, IPv4 addresses, palindromes ignoring punctuation and a simple password policy.
- `04_runLengthAndCompression.cpp` - Run-length encoding and decoding, count-and-say, and in-place style character compression.
- `05_anagramsAndPermutationsInString.cpp` - Anagram problems: valid anagram, group anagrams, find all anagram positions with a window, and permutation-in-string.
- `06_bracketsAndExpressions.cpp` - Stack problems on strings: balanced brackets, longest valid parentheses, minimum additions, generating parentheses and expression evaluation.
- `07_base64HexAndCaesar.cpp` - Text encodings: hexadecimal, Base64 with padding, URL-style percent encoding, Caesar and Vigenere ciphers.
- `08_permutationsAndSubsequences.cpp` - Generating strings: permutations (with duplicates), subsequences, combinations, next_permutation and the kth permutation by factorial numbering.
- `09_slidingWindowStrings.cpp` - Sliding-window string problems: minimum window substring, longest substring with at most k distinct characters and without repeats.
- `10_bigNumberArithmetic.cpp` - Arbitrary-precision integers stored as strings/vectors: addition, subtraction, multiplication and factorial.
- `11_trieAutocomplete.cpp` - A trie of words: insert, search, starts-with, delete, autocomplete suggestions and the longest common prefix.
- `12_naiveAndKmpSearch.cpp` - Substring search: naive O(nm), KMP O(n + m) with the failure function, and the standard library searchers.
- `13_rabinKarpRollingHash.cpp` - Rabin-Karp with a polynomial rolling hash: multi-pattern search and the longest repeated substring by binary search on the length.
- `14_zAlgorithmAndBorders.cpp` - Z-algorithm and the prefix function: pattern search, borders, smallest period and shortest palindrome by prepending.
- `15_boyerMooreHorspool.cpp` - Boyer-Moore-Horspool with a bad-character shift table, counting comparisons against the naive scan.
- `16_manacherPalindromes.cpp` - Palindromic substrings: expand-around-centre O(n^2) and Manacher O(n); longest, count and all centres.
- `17_editDistanceAndLcs.cpp` - String DP: edit distance (with the edit script), LCS, longest common substring, longest palindromic subsequence and the shortest common supersequence length.
- `18_wildcardAndRegexMatching.cpp` - Pattern matching with DP (wildcards ? and *, regex . and *) next to the standard std::regex.
- `19_ahoCorasick.cpp` - Aho-Corasick automaton: search many patterns at once with failure and output links in O(n + m + matches).
- `20_suffixArrayAndLcp.cpp` - Suffix array by prefix doubling (O(n log^2 n)), Kasai's LCP array, longest repeated substring, distinct substrings and pattern search by binary search.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `g++ -std=c++20 -Wall -Wextra -pthread 01_stringBasicsAndViews.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/39_StringAlgorithms/` (git-ignored).

## Key concepts / interview angles
- **Basics:** `std::string` owns a resizable buffer (small-string optimization), `string_view` is a non-owning (pointer, length) view - never return one into a temporary. `find`/`rfind`/`find_first_of` return `npos` on failure; `substr` copies; `replace`, `erase`, `insert`; `compare`; `stoi`/`stod` throw on bad input, `from_chars` does not.
- **Substring search:** naive O(nm); **KMP** O(n+m) with the prefix (failure) function - never moves back in the text; **Z-algorithm** `z[i]` = LCP with the whole string (search on `pat#text`); **Boyer-Moore-Horspool** compares right to left and skips by a bad-character table (sublinear on average); `std::search` with `boyer_moore_searcher` (C++17); **Rabin-Karp** compares rolling hashes (verify on match; multi-pattern friendly).
- **Borders/period:** `pi[n-1]` is the longest border; smallest period = `n - pi[n-1]`; shortest palindrome by prepending = `s + '#' + reverse(s)`, keep the prefix-function value.
- **Aho-Corasick:** trie + BFS failure links + output (dictionary) links -> automaton; all occurrences of all patterns in O(n + m + matches).
- **Palindromes:** expand around each centre (odd and even) O(n^2); **Manacher** O(n) with separators and a mirrored radius inside the rightmost palindrome; counting palindromic substrings = sum of `(p[i]+1)/2`.
- **Suffix array:** sort suffixes (prefix doubling O(n log^2 n) with `std::sort`, O(n log n) with radix sort, SA-IS O(n)); **Kasai** LCP in O(n); longest repeated substring = max LCP; distinct substrings = n(n+1)/2 - sum(LCP); pattern search = binary search over suffixes in O(m log n).

## Gotchas
- Returning a string_view into a temporary string dangles.
- Rolling-hash matches must be verified; hashes can collide.
