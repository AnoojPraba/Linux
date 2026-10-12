# String Algorithms

- **Basics:** `std::string` owns a resizable buffer (small-string optimization), `string_view` is a non-owning (pointer, length) view - never return one into a temporary. `find`/`rfind`/`find_first_of` return `npos` on failure; `substr` copies; `replace`, `erase`, `insert`; `compare`; `stoi`/`stod` throw on bad input, `from_chars` does not.
- **Substring search:** naive O(nm); **KMP** O(n+m) with the prefix (failure) function - never moves back in the text; **Z-algorithm** `z[i]` = LCP with the whole string (search on `pat#text`); **Boyer-Moore-Horspool** compares right to left and skips by a bad-character table (sublinear on average); `std::search` with `boyer_moore_searcher` (C++17); **Rabin-Karp** compares rolling hashes (verify on match; multi-pattern friendly).
- **Borders/period:** `pi[n-1]` is the longest border; smallest period = `n - pi[n-1]`; shortest palindrome by prepending = `s + '#' + reverse(s)`, keep the prefix-function value.
- **Aho-Corasick:** trie + BFS failure links + output (dictionary) links -> automaton; all occurrences of all patterns in O(n + m + matches).
- **Palindromes:** expand around each centre (odd and even) O(n^2); **Manacher** O(n) with separators and a mirrored radius inside the rightmost palindrome; counting palindromic substrings = sum of `(p[i]+1)/2`.
- **Suffix array:** sort suffixes (prefix doubling O(n log^2 n) with `std::sort`, O(n log n) with radix sort, SA-IS O(n)); **Kasai** LCP in O(n); longest repeated substring = max LCP; distinct substrings = n(n+1)/2 - sum(LCP); pattern search = binary search over suffixes in O(m log n).
- **String DP:** edit distance (insert/delete/replace; walk back for the script), LCS, longest common substring (contiguous), longest palindromic subsequence = LCS(s, reverse(s)), shortest common supersequence = |a| + |b| - |LCS|.
- **Wildcards/regex:** DP tables for `?`/`*` and `.`/`x*`; `std::regex` (ECMAScript) offers `regex_match`, `regex_search`, `sregex_iterator`, `regex_replace` but is slow and recursion-heavy.

## More algorithms in this folder
- **Windows with counts:** minimum window substring (`need[]` + `missing`), longest substring with <= k distinct characters, longest without repeats (`last[]` index jump), all anagram positions (fixed window compared by count arrays). Every pointer moves forward only: O(n).
- **Anagrams:** sorted string or 26-count signature as the key to group; permutation-in-string = window equal to the needed counts.
- **RLE/compression:** scan runs with two indexes; count-and-say; RLE only helps long runs - dictionary coders (LZ77/DEFLATE) and entropy coders (Huffman) do the real work.
- **Stack problems:** balanced brackets, longest valid parentheses (stack of indexes with a base), generate parentheses by tracking open/close counts (Catalan numbers), expression evaluation by recursive descent (`expr -> term -> factor`) or shunting-yard.
- **Big numbers:** schoolbook add/multiply on digit arrays (digit i*j lands at i+j+1), factorial by repeated small multiplication; base 10^9 limbs and Karatsuba for speed; production code uses GMP/Boost.Multiprecision.
- **Trie:** array-of-26 children (or a map for large alphabets), `pass` counters for prefix counts, DFS for autocomplete suggestions, common prefix = follow single-child nodes. O(L) operations; memory-hungry (compress into a radix tree).
- **Encodings:** hex (nibbles), **Base64** (3 bytes -> four 6-bit symbols, `=` padding), percent-encoding (unreserved characters stay), Caesar/Vigenere (shift cyphers, not secure). Decode with a bit accumulator.
- **Parsing:** `std::from_chars`/`to_chars` (no exceptions, no locale, locale-independent, fast) over `stoi`/`atoi`; check the returned pointer/`errc`; `istringstream` for typed extraction; CSV needs quote handling (`""` escapes). `iomanip` for width/precision/fill.
- **Validation state machines:** number (sign, digits, one dot, optional exponent with at least one digit), IPv4 (four parts 0-255, no leading zeros), palindrome with two pointers skipping non-alphanumerics, password rules with `any_of`.
- **Generation:** distinct permutations by swap with a duplicate check, `std::next_permutation` (needs a sorted start for all), kth permutation by the factorial number system, subsequences by bitmask, combinations by backtracking.
