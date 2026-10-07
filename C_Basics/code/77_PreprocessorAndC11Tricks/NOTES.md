# Preprocessor Tricks and C99/C11 Features

## Preprocessor
- **X-macros:** one list macro, expanded with different `X(...)` definitions
  to generate an enum, a name table and a switch from a single source of
  truth. The standard answer to "keep these tables in sync."
- `#` stringifies, `##` pastes tokens. Operands of `#`/`##` are NOT
  macro-expanded first - use a two-level wrapper (`STR(x)` -> `STR_(x)`) to
  stringify a macro's value.
- `do { ... } while (0)` makes a multi-statement macro one statement, safe
  under an unbraced `if/else` with a trailing `;`.
- **Double evaluation:** `MAX(i++, j)` evaluates an argument twice. Fixes:
  inline function (type-specific), GNU statement expression + `__typeof__`,
  or `_Generic`.
- Parenthesize every macro parameter and the whole expansion:
  `#define SQ(x) ((x)*(x))`. `SQ(a+1)` without it gives `a+1*a+1`.
- Variadic macros: `#define LOG(fmt, ...) printf(fmt, __VA_ARGS__)` breaks
  with zero args; GNU `##__VA_ARGS__` or C23 `__VA_OPT__` fixes it.
- Include guards vs `#pragma once`; never define non-`static` objects in a
  header (multiple-definition link error).
- Predefined: `__FILE__`, `__LINE__`, `__func__` (a variable, not a macro),
  `__DATE__`, `__STDC_VERSION__`.
- Macros are untyped, unscoped and invisible to debuggers - prefer
  `static inline` functions and `enum`/`const` unless you need token tricks.

## C99/C11 features that show up in senior code
- Designated initializers (`.field =`, `[idx] =`): order-independent, rest
  zeroed; keeps struct init stable when fields are added.
- Compound literals `(T){...}`: unnamed object, lifetime = enclosing block
  (not the full expression!). Returning a pointer to one from a function
  dangles.
- `_Static_assert(cond, "msg")`: compile-time invariant (layout, sizes).
- VLAs: C99 mandatory, C11 optional, never in the Linux kernel (stack
  overflow with no error path); `sizeof` on a VLA is evaluated at run time.
- Anonymous structs/unions (C11), `_Alignas`/`_Alignof` (see
  `../59_MemoryAlignmentAndPadding`), `_Noreturn`, `_Thread_local`.
- `restrict` -> `../23_RestrictQualifier`, `_Generic` -> `../73_GenericMacro`,
  flexible array members -> `../74_ContainerOfAndIntrusiveLists`.
- `inline` semantics differ from C++: a plain `inline` definition emits no
  external symbol in C99+; provide one `extern inline` declaration (or use
  `static inline`) - see `../25_InlineFunctions`.

## Senior interviewer Q&A
**Q: Write a `SWAP` / `MAX` macro and tell me what is wrong with the naive one.**
A: `#define MAX(a,b) ((a)>(b)?(a):(b))` evaluates an argument twice, so
`MAX(i++, j)` increments `i` twice. Fix with a GNU statement expression and
`__typeof__`, an `inline` function, or `_Generic` dispatch. Multi-statement
macros need `do { } while (0)` so they work in `if (x) MACRO(); else ...`.

**Q: Why parenthesize macro arguments and the entire body?**
A: Textual substitution: `#define SQ(x) x*x` makes `SQ(a+1)` become `a+1*a+1`;
and `#define ADD(a,b) a+b` misbehaves in `2*ADD(1,2)`.

**Q: How do you stringify a macro's value, e.g. print `VERSION`'s number?**
A: Two levels: `#define STR_(x) #x` / `#define STR(x) STR_(x)`. Operands of
`#` and `##` are not expanded first; the extra level forces expansion.

**Q: Explain X-macros and why you would use them.**
A: Define one list `X(name, str, code)` and re-expand it with different `X`
definitions to generate the enum, string table, and switch. Adding an entry
is a one-line change, so the parallel tables cannot drift apart - ideal for
error codes, opcodes, state machines, register maps.

**Q: Macros vs `static inline` vs `enum`/`const` - how do you choose?**
A: Prefer `static inline` (typed, debuggable, single evaluation) and
`enum`/`const` for constants. Use macros for what functions cannot do:
token pasting, stringification, `__FILE__`/`__LINE__` capture, generic
control-flow wrappers, conditional compilation.

**Q: What is `_Static_assert` good for?**
A: Compile-time checks on `sizeof`/`offsetof`/array lengths: wire-format
structs (`sizeof(hdr) == 12`), assumptions about `int` width, table sizes
matching enum counts (`ARRAY_SIZE(names) == STATE_COUNT`). Failing the build
beats a runtime surprise.

**Q: Lifetime of a compound literal?**
A: Automatic storage in block scope (static at file scope). So
`return &(struct s){...};` dangles, but passing `&(struct cfg){...}` to a
call within the block is fine and avoids a named temporary.

**Q: What is wrong with variable-length arrays?**
A: Size is runtime-controlled stack allocation with no failure reporting;
a large or attacker-influenced size overflows the stack. The kernel banned
them; C11 made them optional. Use `malloc` with a bound check, or a fixed
maximum.

**Q: How do `static inline` and `inline` differ in C vs C++?**
A: C99 `inline` alone emits no external definition, so you need exactly one
`extern inline` declaration (or make it `static inline` in the header). C++
`inline` allows identical definitions across translation units (ODR).
