# 07_Strings

C string basics: null termination, strlen/strcat/strcpy, hand-written string functions and strtok.

## Files
- `01_stringBasics.c` - strlen vs array size, strcpy/strcat into fixed buffers
- `02_manualStringFunctions.c` - own myStrlen, myStrcpy and myStrrev
- `03_tokenizeString.c` - strtok splitting "the,quick,brown,fox"
- `04_reverseWordsAndRotations.c` - In-place string manipulations: reverse the words of a sentence, rotate a string with the three-reversal trick and test rotation with doubling.
- `05_longestCommonPrefix.c` - Longest common prefix of several strings by vertical scan, sorting (compare first and last) and divide and conquer.
- `06_runLengthEncodingAndCompression.c` - Run-length encoding and decoding, plus in-place character compression like a3b2.
- `07_validNumberAndIpAddress.c` - Validating formatted strings: numbers (sign, decimals, exponent) with a state machine, IPv4 addresses and a palindrome ignoring punctuation.
- `08_validParenthesesAndExpressions.c` - Stack-based string problems: balanced brackets, longest valid parentheses, minimum additions and evaluating a simple expression.
- `09_isomorphicAndWordPattern.c` - Bijection checks between strings: isomorphic strings, word pattern matching and valid anagram groups by signature.
- `10_permutationsAndSubsequences.c` - Generating strings: all permutations (with and without duplicates), all subsequences with bitmasks, and the next lexicographic permutation.
- `11_caesarVigenereAndBase64.c` - Classic string encodings: Caesar and Vigenere ciphers and Base64 encoding/decoding with bit shifts.
- `12_minimumWindowSubstring.c` - Sliding window with character counts: minimum window containing all characters of a target, and the longest substring with at most k distinct characters.
- `13_editDistanceAndLcs.c` - String DP trio: edit distance (Levenshtein), longest common subsequence and longest common substring.
- `14_bigNumberStringArithmetic.c` - Arbitrary-precision arithmetic on decimal strings: addition, multiplication and factorial of large numbers.

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_stringBasics.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/07_Strings/` (git-ignored).

## Key concepts / interview angles
- strlen counts bytes up to `'\0'`, not the array size.
- strcpy/strcat do no bounds checking; prefer snprintf or sized variants.
- `strtok` mutates its input and keeps hidden static state (not reentrant; use `strtok_r`).
- Reversing in place is a classic two-pointer problem.

## Gotchas
- `03_tokenizeString.c` uses a `char[]` (not a literal) because strtok writes into the buffer.

## Related
- `../03_pointers/08_pointersAndStrings.c`
- `../58_ItoaAtoiSafeParsing`
- `../11_StringPatternMatching`
- `../65_SecurityDemos` - buffer overflows

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
