# Safe Integer Parsing/Formatting (atoi/itoa pitfalls)

Builds on the signed-overflow-is-UB background in `52_UndefinedBehaviorCatalog`
with two very common interview traps: writing a
correct bounded `atoi` and a correct `itoa`.

## Why "multiply then check for overflow" is itself UB

- The naive pattern `result = result * 10 + digit; if (result < prev) { /*
  overflowed */ }` looks reasonable but is undefined behavior for signed
  integers: the overflow has *already happened* by the time the multiply
  executes. The compiler is free to assume signed overflow never occurs and
  optimize the "check" away entirely, or produce any other behavior.
- The correct pattern checks the bound **before** the operation that would
  overflow, e.g. `if (result > (INT_MAX - digit) / 10) { /* would overflow */ }`
  computed with values that are all guaranteed in-range, so no UB occurs
  even while detecting the would-be overflow.
- `01_safeAtoi.c` applies this pre-check pattern, and signals errors via an
  `errno`-style out-parameter (`ATOI_OK`/`ATOI_ERR_INVALID`/
  `ATOI_ERR_OVERFLOW`/`ATOI_ERR_UNDERFLOW`) rather than silently returning
  garbage - the standard `atoi()` has no error signaling at all, which is
  exactly the footgun this file avoids.

## The `INT_MIN` negation trap

- `INT_MIN` (e.g. -2147483648 for 32-bit int) has no positive counterpart
  that fits in a signed int - `-INT_MIN` would need to represent 2147483648,
  which overflows `INT_MAX` (2147483647). Writing `-value` when `value` could
  be `INT_MIN` is undefined behavior.
- Both files here sidestep this by working with the **unsigned magnitude**:
  casting to `unsigned int` before negating relies on well-defined modular
  unsigned arithmetic (`(unsigned int) 0 - (unsigned int) value`), which
  produces the correct magnitude for every `int` value, `INT_MIN` included.
  `01_safeAtoi.c` similarly accumulates into an `unsigned int` and only
  applies the sign at the very end.

## Efficient itoa

- `02_itoaFast.c` writes digits least-significant-first (from repeated
  `% base` / `/ base`) directly into the destination buffer, then reverses
  the buffer once at the end - O(n) total. Repeatedly prepending digits (or
  concatenating with something like `strcat` in a loop) would be O(n^2).
- Supports arbitrary base 2-36 via a `"0123456789abcdefghijklmnopqrstuvwxyz"`
  digit table indexed by `magnitude % base`.
