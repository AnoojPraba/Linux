# 05_BitManipulation

Bit manipulation interview toolkit: single-bit idioms, XOR tricks, popcount, power-of-two, bit reversal, rotation and CRC-8; moderate to senior depth with a quick-reference NOTES.md.

## Files
- `01_BitManipulation.c` - rearranges nibbles of 0xDCBA into 0xBACD with shifts and masks
- `02_Boolean.c` - enum-based boolean type
- `03_OddOrEven.c` - `n & 1` parity test (reads stdin)
- `04_get_bitset.c` - prints binary representation of a number given hex or decimal, grouped; uses ../common/utils.h
- `05_countSetBits.c` - naive shift loop vs Kernighan `n & (n-1)` popcount
- `06_isPowerOfTwo.c` - `n && !(n & (n-1))` test
- `07_xorSwap.c` - swap without a temp; fails when both pointers refer to the same object
- `08_singleNonRepeating.c` - XOR cancels pairs to find the single element
- `09_missingNumber.c` - XOR of elements and 1..n finds the missing number
- `10_bitIdioms.c` - set / clear / toggle / test a bit
- `11_reverseBits.c` - reverse the bits of a uint32
- `12_bitRotation.c` - rotateLeft/rotateRight by k (wrap instead of discard)
- `13_crc8Checksum.c` - bitwise CRC-8 (poly 0x07, init 0x00)
- `NOTES.md` - sections: classic tricks 05-12, CRC checksum, single-bit idiom quick reference, why XOR tricks work, why asked often

## Build and run
- `04_get_bitset.c` includes `common/utils.h`; the Makefile builds with `-I.` from `C_Basics/code`. By hand from this folder: `gcc -Wall -Wextra -std=gnu11 -I.. 04_get_bitset.c -o /tmp/x && /tmp/x 0xF0F0` (the number, hex with 0x or decimal, is argv[1]).
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_BitManipulation.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/05_BitManipulation/` (git-ignored).

## Key concepts / interview angles
- Kernighan: `n &= n - 1` clears the lowest set bit (popcount, power of two).
- XOR is self-inverse: `x ^ x = 0`, `x ^ 0 = x`; basis of single-number, missing-number and swap.
- XOR swap fails when both operands alias the same object (result 0).
- Use unsigned types for shifts; shifting a signed negative or by >= width is UB/implementation-defined.
- Rotation = `(n << k) | (n >> (32 - k))`, mask k to avoid a shift by 32.
- CRC-8: shift register, XOR polynomial on MSB.

## Gotchas
- `03_OddOrEven.c` reads stdin; `04_get_bitset.c` requires the number as argv[1] (returns -1 silently without it) and needs the `-I..` include path.

## Related
- `../common/utils.h` - hex/decimal helper header used by 04
- `../06_EndiannessAndByteOrder`
- `../27_BitFields`
- `../75_IntegerPromotionsAndConversions`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
