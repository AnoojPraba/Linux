# 07_Strings

## More algorithms in this folder
- **In-place tricks:** reverse a sentence's words = reverse the whole string, then reverse each word; rotate left by k = three reversals (`rev(0,k-1) rev(k,n-1) rev(0,n-1)`), O(n) time, O(1) space; `b` is a rotation of `a` iff `b` is a substring of `a+a` with equal length.
- **Run-length encoding:** scan runs with two pointers; compress in place with a separate write index (omit counts of 1). Decode with `strtol` for the multi-digit count.
- **Brackets/expressions:** a stack of openers matches closers; longest valid parentheses with a stack of indexes (base index -1) or two scans; minimum additions = unmatched open + unmatched close; arithmetic by **recursive descent** (`expr -> term -> factor`) or the shunting-yard algorithm; precedence = grammar depth.
- **Common prefix:** vertical scan O(total chars), sort and compare only first/last, divide and conquer, or a trie.
- **String DP:** **edit distance** `dp[i][j] = min(del, ins, sub)` O(nm) (space O(min) with two rows); **LCS** `dp[i][j] = match ? diag+1 : max(up, left)`, rebuild by walking back; **longest common substring** is the contiguous variant (no max with neighbours, track the best cell). Also: longest palindromic subsequence = LCS(s, reverse(s)).
- **Big numbers on strings:** add with a carry from the right; multiply schoolbook (digit i*j lands at i+j+1, carry into i+j); factorial with a digit array. For speed use base 10^9 limbs or Karatsuba.
- **Window with counts:** minimum window substring (`need[]` counts and a `missing` counter, shrink while valid), longest substring with at most k distinct characters, longest substring without repeats. O(n) because both pointers only move forward.
- **Bijections:** isomorphic strings need a mapping that is one-to-one in BOTH directions (two maps); word pattern is the same with words; group anagrams by sorted-string or 26-count signature.
- **Generation:** permutations by swap-and-backtrack (skip a character already used at this position to avoid duplicates), subsequences by bitmask (2^n), next permutation = rightmost ascent, swap with the next larger on the right, reverse the suffix.
- **Encodings:** Caesar `(c - base + k) mod 26`; Vigenere = Caesar with a repeating key; **Base64** packs 3 bytes (24 bits) into four 6-bit symbols, `=` pads the last group; decode accumulates 6 bits at a time. Validation = state machines (number: sign, digits, one dot, optional exponent; IPv4: four 0-255 parts without leading zeros).
