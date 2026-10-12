# Bit Manipulation

## Classic interview tricks (`05`-`12`)

- `05_countSetBits.c`: naive shift-and-check (O(number of bits)) vs Brian
  Kernighan's `n & (n - 1)` (O(number of set bits), since each iteration
  clears exactly the lowest set bit), plus `__builtin_popcount()` as the
  compiler-intrinsic fast path (often a single hardware POPCNT instruction).
- `06_isPowerOfTwo.c`: `(n > 0) && ((n & (n - 1)) == 0)`. A power of two has
  exactly one bit set, and `n - 1` flips that bit off while setting every
  lower bit, so ANDing the two clears that one bit and yields zero.
- `07_xorSwap.c`: swap two integers with three XORs and no temp variable.
  Fun trick, but modern compilers make a normal temp-variable swap just as
  fast (or faster) in practice, and XOR swap breaks if both arguments alias
  the same memory location (the first XOR zeroes it out).
- `08_singleNonRepeating.c`: XOR every element of an array where all but one
  value appears twice - pairs cancel to zero, leaving only the unique value.
  O(n) time, O(1) space.
- `09_missingNumber.c`: XOR every array element with every number 1..n;
  matching pairs cancel, leaving the missing number. Contrast with the
  sum-formula approach (`sum(1..n) - actual sum`): XOR avoids the integer
  overflow the sum approach risks for large n.
- `10_bitIdioms.c`: the four standard single-bit idioms - set
  (`n | (1 << pos)`), clear (`n & ~(1 << pos)`), toggle (`n ^ (1 << pos)`),
  check (`(n >> pos) & 1`).
- `11_reverseBits.c`: reverse a 32-bit unsigned integer's bit order by
  walking all 32 bits, shifting the result left and ORing in each bit of n
  from least to most significant.
- `12_bitRotation.c`: rotate-left/right for a fixed 32-bit width, wrapping
  bits shifted off one end back onto the other (`(n << k) | (n >> (32 - k))`
  and its mirror), unlike a plain shift which discards them. Used in some
  hash functions and cryptographic algorithms.

## CRC checksum (`13_crc8Checksum.c`)

CRC (Cyclic Redundancy Check) detects - it does NOT correct - bit errors
introduced during transmission or storage. It's the standard checksum in
UART/CAN/Ethernet framing and storage formats. `13_crc8Checksum.c` implements
CRC-8 (polynomial 0x07) with the classic bitwise algorithm: XOR the message
byte into the CRC register, then shift 8 times, XORing in the polynomial
whenever the shifted-out bit was 1. It computes the CRC of a test message,
flips one bit in a copy, recomputes the CRC, and shows the two differ -
proving the check would catch that corruption. A table-driven version
(a precomputed 256-entry lookup table) is the faster real-world alternative,
trading a small ROM table for far fewer runtime operations - common in
embedded communication stacks - but isn't implemented here.

Compared to weaker/different alternatives:
- **Parity bit**: much weaker than CRC - a single parity bit only detects an
  odd number of flipped bits; two flips cancel out and go unnoticed. CRC's
  multi-bit polynomial division catches far more error patterns (all
  single-bit errors and most burst errors, for typical polynomials).
- **Cryptographic hash**: CRC is NOT designed to resist deliberate tampering,
  only accidental corruption. It's trivial to construct a different message
  with the same CRC on purpose, so never use CRC for security/integrity
  purposes - use a cryptographic hash (e.g. SHA-256) for that instead.

## Quick reference: single-bit idioms

| Operation | Formula                |
|-----------|-------------------------|
| Set       | `n \| (1 << pos)`       |
| Clear     | `n & ~(1 << pos)`       |
| Toggle    | `n ^ (1 << pos)`        |
| Check     | `(n >> pos) & 1`        |

## Why the XOR tricks work

XOR is its own inverse: `a ^ a = 0` and `a ^ 0 = a`. That's the entire
mechanism behind XOR swap, single-non-repeating-element, and
missing-number: XORing a value with itself cancels it out, and XORing
anything with 0 leaves it unchanged, so XORing a whole collection together
cancels every duplicate and leaves only whatever is unpaired.

## Why these questions are asked so often

These bit tricks are small, self-contained, and easy to verify by hand or
with a quick test run, yet they immediately reveal whether a candidate is
comfortable reasoning about numbers at the bit level rather than only at
the level of arithmetic - which is exactly why they show up so often in
interviews.

## More algorithms in this folder
- **Core one-liners:** `x & -x` isolates the lowest set bit; `x & (x-1)` clears it (Kernighan popcount, power-of-two test `x && !(x & (x-1))`); `x | (x-1)` sets trailing zeros; `~x & (x+1)` isolates the lowest zero; `x ^ (x>>1)` = Gray code; `(37+7) & ~7` rounds up to a multiple of 8; `n & (2^k - 1)` = `n % 2^k`.
- **Branch-free:** `mask = (a-b) >> 31` (arithmetic shift) gives min/max/abs without a branch; average without overflow `(a & b) + ((a ^ b) >> 1)`; xor swap breaks if both operands alias. Right shift of a negative signed int is implementation-defined; shifting by >= width or left-shifting into the sign bit is UB - use unsigned types.
- **Subsets:** iterate `mask` in `[0, 2^n)`; all **submasks** of `m` with `sub = (sub - 1) & m` (total 3^n over all masks); fixed-size subsets with **Gosper's hack** `c = x & -x; r = x + c; x = (((r ^ x) >> 2) / c) | r`.
- **Gray code:** `n ^ (n >> 1)`; neighbours differ in exactly one bit (rotary encoders, Karnaugh maps); decode = prefix xor of the bits; reflected construction mirrors the previous list and sets a new top bit.
- **Unique-number tricks:** xor cancels pairs (one single); with two singles split by any set bit of `a ^ b`; with triples keep per-bit counts mod 3 (`ones`/`twos`); missing + duplicate combine xor of the array with xor of 1..n then split by a set bit.
- **Counting bits:** Kernighan (cost = number of set bits), 8-bit lookup table, **SWAR** parallel add (`0x55555555`, `0x33333333`, `0x0F0F0F0F`, multiply by `0x01010101`), `__builtin_popcount` (a single `popcnt`/`cnt` instruction). Parity by folding xor; Hamming distance = popcount(a ^ b). `__builtin_clz/ctz` on 0 is undefined.
- **Bitmask DP:** state = set of used items (`dp[mask]`), the next person = `popcount(mask)`; assignment problem O(2^n n), TSP O(2^n n^2); valid up to n ~ 20-24. Derangements and counting orderings use the same pattern.
- **Binary trie for XOR:** insert numbers MSB first; for each query walk the opposite bit when it exists to maximize the xor; max subarray xor = max over prefix-xor pairs. O(n * bits).
- **Bit arrays:** one bit per element (`a[i>>3] |= 1 << (i&7)`): sieve of Eratosthenes in n/8 bytes (start crossing at i*i), bloom filters, bitmaps for free blocks. **Arithmetic from bits:** add = xor + (and << 1) carry loop, multiply = shift-and-add, divide = binary long division.
- **Fields and flags:** permission masks (`READ|WRITE`, test `(x & m) == m` for all-of), packing dates/colours into integers by shifts and masks; extract bytes by shifting for endianness-independent code, by pointer cast to observe the machine's order.
