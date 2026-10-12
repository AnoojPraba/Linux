# 40_BitManipulation

Bit manipulation in modern C++: the C++20 <bit> header, bit tricks, std::bitset, subsets and submasks, Gray code, unique-number tricks, bitmask DP, XOR tries, flags and bit-fields.

## Files
- `01_bitHeaderStdBit.cpp` - The C++20 <bit> header: popcount, countl_zero/countr_zero, bit_width, bit_ceil/bit_floor, has_single_bit, rotl/rotr, bit_cast and endian.
- `02_bitTricksCatalog.cpp` - A catalog of one-line bit tricks in C++: lowest set bit, clear lowest, branch-free min/max/abs, sign, swap, average without overflow and field extraction.
- `03_popcountAlgorithms.cpp` - Population count four ways: Kernighan, lookup table, SWAR parallel add and std::popcount, with Hamming distance and parity.
- `04_bitsetAndVectorBool.cpp` - std::bitset and vector<bool>: set/reset/flip/test, counting, shifts, conversions and bitset as a compact set of small integers.
- `05_flagsEnumsAndBitfields.cpp` - Bit flags in modern C++: enum class with operator overloads, bit-fields, packing fields into an integer and flag sets with std::bitset.
- `06_powersAndLogarithms.cpp` - Powers of two and logarithms with bit scans: is power of 2/4, next power of two, floor(log2), bit length, integer square root and fast exponentiation by squaring.
- `07_reverseBitsAndRotations.cpp` - Reversing and rotating bits: reverse a word (loop, lookup, divide and conquer), swap nibbles/bytes, rotate and parity.
- `08_bitwiseProblems.cpp` - Classic bitwise interview problems: counting bits for 0..n, sum of two integers, maximum product of word lengths, xor queries and bit-flip counts.
- `09_singleNumberVariants.cpp` - XOR and counting tricks for unique elements: single, two singles, single among triples, missing and duplicate numbers.
- `10_grayCode.cpp` - Gray code: to and from binary, the reflected construction and the rotary-encoder use case; neighbours differ in one bit.
- `11_subsetsAndSubmasks.cpp` - Enumerating subsets with bitmasks: all subsets, all submasks of a mask, fixed-size subsets (Gosper's hack with <bit>) and subset-sum counting.
- `12_arithmeticWithoutOperators.cpp` - Arithmetic from bit operations only: add, subtract, multiply, divide and compare, with unsigned types to avoid undefined behaviour.
- `13_bitsetSieve.cpp` - Sieve of Eratosthenes on a std::bitset / packed bits and the memory difference to a byte-per-number sieve.
- `14_maxXorTrie.cpp` - Maximum XOR with a binary trie: best pair, best partner for a query and the maximum xor subarray.
- `15_bitmaskDpAndSos.cpp` - Bitmask DP: minimum-cost assignment, derangements by masks and the sum-over-subsets (SOS) DP that sums a function over all submasks in O(n 2^n).
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `g++ -std=c++20 -Wall -Wextra -pthread 01_bitHeaderStdBit.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/40_BitManipulation/` (git-ignored).

## Key concepts / interview angles
- **<bit> (C++20):** `popcount`, `countl_zero`/`countr_zero` (defined for 0: return the width), `bit_width` (= floor(log2 x) + 1), `bit_floor`/`bit_ceil`, `has_single_bit`, `rotl`/`rotr`, `bit_cast` (UB-free type punning), `std::endian`. All `constexpr`, compile to single instructions where available. Use unsigned types: shifting a negative or by >= width is UB, right-shifting negatives was implementation-defined before C++20.
- **One-liners:** `x & -x` lowest set bit; `x & (x-1)` clear it; power of two = `has_single_bit`; round up to a multiple of 2^k `(x + m) & ~m`; average `(a & b) + ((a ^ b) >> 1)`; branch-free min/max/abs with the sign mask; xor swap fails when both operands alias.
- **`std::bitset<N>`** is a fixed-size bit array (set/reset/flip/test/count/any/none/all, shifts, `to_string`/`to_ulong`, string constructor throws on bad characters); good as a set of small integers and for sieves (`125 KB` for a million flags). **`vector<bool>`** is packed too but its proxy reference is not a real `bool&` - prefer `bitset` or `vector<char>`.
- **Subsets:** masks `[0, 2^n)`; submasks `sub = (sub - 1) & m`; fixed-popcount subsets by **Gosper's hack**; counting pairs (mask, submask) = 3^n; **SOS DP** sums a function over all submasks of every mask in O(n 2^n) (iterate bits, add `f[mask ^ bit]`).
- **Gray code** `n ^ (n >> 1)`; decode by prefix xor; reflected construction; neighbours differ in one bit (rotary encoders, Karnaugh maps).
- **Unique numbers:** xor for a single among pairs; split by a set bit of `a ^ b` for two singles; per-bit counters mod 3 (`ones`, `twos`) for triples; missing/duplicate via xor of indexes and values; `a ^ b` popcount = bits to flip.

## Gotchas
- Use unsigned types for shifts; shifting by >= the width or left-shifting into the sign bit is undefined behaviour.
- Bit-field layout is implementation-defined; use explicit shifts and masks for wire formats.
