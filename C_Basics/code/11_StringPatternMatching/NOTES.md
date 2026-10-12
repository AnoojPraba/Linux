# String Pattern Matching

- Naive search is O(n*m) worst case: for each of the ~n starting positions it
  can compare up to m characters before failing. A repetitive pattern like
  "aaab" against a text of "a"s (e.g. "aaaa...aab") triggers this - almost
  every position matches m-1 characters before the final mismatch.
- KMP avoids redundant comparisons using the failure function (LPS array):
  `lps[i]` is the length of the longest proper prefix of `pattern[0..i]`
  that is also a suffix of it. On a mismatch, instead of restarting the
  pattern pointer from 0, KMP jumps it to `lps[j - 1]` - reusing the fact
  that the already-matched characters tell us exactly how much of the
  pattern could still line up. The text pointer never moves backward, so
  total time is O(n + m).
- Rabin-Karp uses a rolling hash: the hash of the next text window is
  computed from the current window's hash in O(1) (subtract the leading
  character's weighted contribution, shift, add the trailing character),
  rather than rehashing the whole window each time. Hashes are compared
  first; only on a hash match does it fall back to a full character
  comparison (handles hash collisions, keeping the algorithm correct).
  Average case O(n + m), worst case O(n*m) if collisions are frequent.
- When each is preferred:
  - Naive: fine for short/one-off searches where simplicity beats
    performance.
  - KMP: guaranteed linear time regardless of input, general-purpose choice
    when worst-case matters.
  - Rabin-Karp: shines when searching for multiple patterns at once (hash
    each pattern once, then scan the text once comparing against all of
    them) - e.g. plagiarism detection / duplicate-substring style matching.

## More algorithms in this folder
- **Z-algorithm:** `z[i]` = longest common prefix of `s` and `s[i..]`; maintain the rightmost Z-box `[l, r)` and reuse `z[i-l]` inside it - linear. Pattern search: build `pattern + '#' + text` and report positions where `z == len(pattern)`. Also gives periods and borders.
- **KMP prefix function applications:** `pi[n-1]` is the longest border; smallest period `n - pi[n-1]` (a repetition if it divides n); all borders by following `pi[k-1]`; shortest palindrome by prepending = `s + '#' + reverse(s)` then keep `pi[last]`; occurrences count via `pi` on `pattern#text`.
- **Boyer-Moore-Horspool:** compare the pattern right to left; on a mismatch shift by the bad-character table entry of the text character under the pattern's last position (default = pattern length). Average sublinear (O(n/m)), worst case O(nm). Full Boyer-Moore adds the good-suffix rule for O(n) worst case with Galil.
- **Aho-Corasick:** trie of all patterns + BFS-built **failure links** (longest proper suffix that is a trie node) and **dictionary links** (nearest pattern end), turning the trie into an automaton: one pass over the text reports every occurrence of every pattern in O(n + m + matches). Used by `fgrep`, intrusion detection and profanity filters.
- **Manacher:** longest/all palindromic substrings in O(n) by inserting separators (so even and odd palindromes share a centre), keeping the rightmost palindrome `(centre, right)` and mirroring radii. Expand-around-centre is O(n^2) but simpler.
- **Suffix array + LCP:** sort all suffixes (`qsort` O(n^2 log n) here, prefix doubling O(n log n), SA-IS O(n)); **Kasai** builds LCP in O(n) using `h` that drops by at most 1 per step. Longest repeated substring = max LCP; distinct substrings = n(n+1)/2 - sum(LCP); substring search by binary search on the array. Suffix automata and suffix trees solve the same problems.
- **Rolling hash:** `h = (h*B + c) mod M`, remove the outgoing character with `B^len`; Rabin-Karp search, duplicate-window and **longest repeated substring by binary search on the length** (monotone predicate). Always verify on a hash match, use a large modulus or two moduli to make collisions unlikely, and watch unsigned overflow.
- **Wildcard/regex DP:** wildcard `dp[i][j]` = first i chars match first j pattern chars; `*` = `dp[i][j-1] || dp[i-1][j]`, `?` and literals use the diagonal. Regex `x*` = zero uses (`skip p+2`) or one use and stay; memoize to avoid exponential time.
