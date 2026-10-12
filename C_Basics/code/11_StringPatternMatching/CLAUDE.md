# 11_StringPatternMatching

Substring search algorithms: naive, KMP and Rabin-Karp, with complexity and when-to-use trade-offs.

## Files
- `01_naiveSearch.c` - brute force, O(n*m) worst case on "aaaa..ab" style inputs
- `02_kmpSearch.c` - builds the LPS (failure) array; text pointer never moves back, O(n+m)
- `03_rabinKarpSearch.c` - rolling hash with full compare on hash match; average O(n+m)
- `04_zAlgorithm.c` - Z-algorithm: the Z-array of a string and linear-time pattern search with the pattern#text trick.
- `05_prefixFunctionApplications.c` - Applications of the KMP prefix function: borders, smallest period, counting occurrences and the shortest palindrome prefix extension.
- `06_boyerMooreHorspool.c` - Boyer-Moore-Horspool: compare the pattern right to left and skip ahead with a bad-character table; sublinear on typical text.
- `07_rollingHashRepeatedSubstring.c` - Polynomial rolling hash plus binary search on length: longest repeated substring and a duplicate-window finder.
- `08_wildcardAndRegexMatching.c` - Pattern matching with DP: wildcards ('?' and '*') and a small regular-expression matcher ('.' and '*' as repetition).
- `09_manacherPalindromes.c` - Manacher's algorithm: all palindromic substrings and the longest one in O(n), compared with expand-around-centre.
- `10_ahoCorasick.c` - Aho-Corasick: find all occurrences of many patterns in one pass using a trie with failure links.
- `11_suffixArrayAndLcp.c` - Suffix array (sorted suffixes) and Kasai's LCP array: longest repeated substring and distinct-substring count.
- `NOTES.md` - complexity and when each is preferred (multi-pattern search favours Rabin-Karp)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_naiveSearch.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/11_StringPatternMatching/` (git-ignored).

## Key concepts / interview angles
- `lps[i]` = longest proper prefix of `pattern[0..i]` that is also a suffix; on mismatch jump pattern index to `lps[j-1]`.
- Rabin-Karp worst case degrades to O(n*m) with many collisions; always verify on hash match.
- Rabin-Karp shines for multi-pattern search (plagiarism / duplicate detection).
- Naive is fine for short, one-off searches.

## Related
- `../07_Strings`
- `../44_Trie` - prefix structure for many patterns
- `../50_ClassicArrayAndStringProblems`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
