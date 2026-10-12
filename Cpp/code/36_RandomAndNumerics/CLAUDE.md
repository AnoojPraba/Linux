# 36_RandomAndNumerics

<random> engines and distributions, floating-point comparison, numeric_limits, overflow checks and <cmath>.

## Files
- `01_randomVsRand.cpp` - srand/rand versus mt19937 + uniform_int/real_distribution, shuffle, random_device.
- `02_randomDistributions.cpp` - mt19937/mt19937_64 first outputs, uniform/normal/bernoulli distributions checked by properties, seed_seq seeding.
- `03_floatingPointAndLimits.cpp` - 0.1+0.2, abs+relative tolerance compare, numeric_limits, NaN/inf, signed overflow check, midpoint/lerp.
- `04_cmathFunctions.cpp` - sqrt, pow, floor/ceil/round/trunc on +/- 2.5, fmod, trig/log, integer vs floating division, NaN/inf tests.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `g++ -std=c++20 -Wall -Wextra -pthread 01_randomVsRand.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/36_RandomAndNumerics/` (git-ignored).

## Key concepts / interview angles
- **Engine vs distribution:** the engine (`mt19937`: 2.5 KB state, good quality, not cryptographic) yields raw bits and is exactly specified (10000th output of a default `mt19937` is 4123659995; first output for seed 5489 is 3499211612). Distributions (`uniform_int_distribution`, `normal_`, `bernoulli_`, `discrete_`) are NOT specified bit-for-bit, so test properties (range, mean, histogram), not exact values.
- **Why not `rand() % n`:** hidden global state (not thread-safe), implementation-defined quality (weak low bits), small `RAND_MAX`, modulo bias. Seed once and keep the engine (creating one per call gives repeated numbers); one engine per thread. Seed from `std::random_device`, mix the whole state with `std::seed_seq`. Fixed seed in tests. Nothing in `<random>` is cryptographic - use `getrandom`/a crypto library.
- **Floating point:** most decimals are inexact; never `==` after arithmetic - use absolute + relative tolerance. `epsilon()` is the gap above 1.0, not a universal tolerance. **NaN** is unordered (`nan == nan` false; sorting NaNs with `<` is UB); `inf - inf` and `0.0/0.0` are NaN; integer division by zero is UB. `float` has 24 bits mantissa (2^24 + 1 == 2^24). Kahan/pairwise summation; never floats for money.
- **Integers:** signed overflow is UB (optimizer assumes it never happens); unsigned wraps. Check before the operation or use `__builtin_add_overflow`. `std::midpoint`, `std::lerp` (C++20). `numeric_limits<double>::min()` is the smallest *positive* value, `lowest()` the most negative.
- **<cmath>:** `round` rounds halves away from zero; `floor` toward -inf; `ceil` toward +inf; `trunc` toward 0; `%` is integer-only (`fmod` for doubles), sign follows the dividend; `std::abs` on a double (from `<cmath>`) not the int `abs`.

## Gotchas
- `-ffast-math` breaks NaN/inf handling and reassociates sums.
- `char` signedness is platform dependent (aarch64 Linux: unsigned).
