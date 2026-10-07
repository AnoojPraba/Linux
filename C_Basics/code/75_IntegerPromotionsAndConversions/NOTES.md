# Integer Promotions and Conversions

- **Integer promotion:** any operand of rank lower than `int` (`char`, `short`,
  `_Bool`, bit-fields) becomes `int` (or `unsigned` if `int` cannot hold all
  its values) before arithmetic, bitwise and shift operators.
  `~(uint8_t)0x0F` is `0xFFFFFFF0`, not `0xF0`.
- **Usual arithmetic conversions** (after promotion), in order:
  1. same type -> done;
  2. same signedness -> convert to the higher rank;
  3. unsigned rank >= signed rank -> convert signed to unsigned (the `-1 < 1u`
     trap: `-1` becomes `UINT_MAX`);
  4. signed type can represent all unsigned values -> convert to signed
     (`long` vs `unsigned int` on LP64);
  5. otherwise both convert to the unsigned counterpart of the signed type.
- Signed overflow is UB; unsigned arithmetic wraps modulo 2^N by definition.
  Converting an out-of-range value TO a signed type is implementation-defined
  (GCC wraps); TO unsigned is always defined (modulo).
- Classic bugs:
  - `for (size_t i = n - 1; i >= 0; i--)` never ends - use `i-- > 0`.
  - `if (len - used < 0)` with unsigned operands - always false.
  - `sizeof x > -1` - `-1` converts to `SIZE_MAX`, so false.
  - `memcpy(dst, src, n)` where `n` is a negative `int` -> huge `size_t`.
  - `abs(INT_MIN)`, `-INT_MIN`, `INT_MIN / -1`: UB.
  - `char` signedness differs (signed on x86, unsigned on ARM Linux): cast to
    `unsigned char` before `isalpha()`/table indexing.
- Shifts: shifting by >= width or a negative count is UB; left shift of a
  negative signed value is UB (before C23); `1 << 31` on 32-bit `int` is UB -
  use `1u << 31`.
- `float`/`double` -> integer truncates toward zero; out-of-range is UB.
- Variadics: `float` promotes to `double`, small ints to `int` (default
  argument promotions) - why `printf("%f")` takes a `double` and `va_arg(ap, char)`
  is a bug.
- **Sequence points:** modifying an object twice (or modifying and reading it
  for other than computing the new value) between sequence points is UB:
  `i = i++ + 1`, `a[i] = i++`. Function-argument evaluation order is
  unspecified. `&&`, `||`, `?:`, `,` and the end of a full expression are
  sequence points (C11 phrases this as "sequenced before").
- Habit: compile with `-Wall -Wextra -Wconversion -Wsign-compare` and use
  fixed-width types (`<stdint.h>`) for wire formats.

## Senior interviewer Q&A
**Q: What does `unsigned int u = 1; int s = -1; s < u` give, and why?**
A: 0. The usual arithmetic conversions turn `s` into `unsigned`
(4294967295). Fix by comparing explicitly (`s < 0 || (unsigned)s < u`) or using
signed types for lengths where negative is meaningful. Build with
`-Wsign-compare`.

**Q: Why is `for (size_t i = n - 1; i >= 0; i--)` a bug?**
A: `i >= 0` is always true for unsigned; it wraps to `SIZE_MAX` and indexes
out of bounds. Use `for (size_t i = n; i-- > 0;)`. If `n == 0`, `n - 1` also
wraps - the idiom above handles that.

**Q: What does `~(uint8_t)0x0F` evaluate to?**
A: `0xFFFFFFF0` as an `int` - integer promotion happens before `~`. Cast back
to `uint8_t` to get `0xF0`. Same reason `uint8_t << 24` can hit UB: the
promoted `int` may overflow into the sign bit; use `(uint32_t)b << 24`.

**Q: Is `x + 1 > x` always true? For which types?**
A: For signed `int` the compiler may assume it (overflow is UB) and fold it to
true; for `unsigned` it is false when `x == UINT_MAX`. This is why overflow
checks like `if (a + b < a)` are unreliable for signed values. Use
`__builtin_add_overflow` / check against limits first.

**Q: How do you safely check `a * b` fits in `size_t`?**
A: `if (b != 0 && a > SIZE_MAX / b) fail;` or `__builtin_mul_overflow`.
Relevant to `malloc(n * size)` - why `calloc` and `reallocarray` exist.

**Q: Does `char c = 200;` behave the same everywhere?**
A: No. Plain `char` is signed on x86, unsigned on ARM Linux; out-of-range
conversion to a signed type is implementation-defined. Use `uint8_t` /
`unsigned char` for bytes and cast to `unsigned char` before `isalpha`.

**Q: `i = i++ + 1` - what happens?**
A: Undefined behavior: `i` is modified twice (the `++` side effect and the
assignment) with no sequencing between them (pre-C11: no intervening sequence point). Likewise
`a[i] = i++` and `f(i++, i++)` (argument evaluation order is unspecified).

**Q: What is `-INT_MIN`, `abs(INT_MIN)`, `INT_MIN / -1`?**
A: All signed overflow = UB (and `INT_MIN / -1` traps with SIGFPE on x86).
Guard before negating or divide in a wider type.

**Q: Why does `printf("%f", 1.0f)` work but `%d` with a `long` does not?**
A: Default argument promotions turn `float` into `double` for variadics, so
`%f` consumes a double. `%d` with `long` mismatches width: UB, and wrong on
LP64 because registers/stack slots are 64-bit. Use `%ld` / `%zu` / `PRId64`.
