# C11 `_Generic` Type-Generic Macros

Notes-focused: `28_Generics/01_genericMacro.c` already is the runnable
`_Generic` demo (a type-generic `add()` dispatching to `addInt`/`addFloat`/
`addDouble`, plus a `typeName()` describer macro) - this folder does not
re-demo that code, it goes deeper into the selection mechanics and contrasts
`_Generic` with the older void*-based style of "generic" C code.

## How `_Generic` selection works

- `_Generic(controlling-expr, type1: expr1, type2: expr2, ..., default:
  exprN)` is evaluated entirely at **compile time**: the compiler looks at
  the *type* of `controlling-expr` (after the usual promotions - see
  below), matches it against the listed type associations, and the whole
  `_Generic` expression is replaced by the single matching branch. The
  branches that don't match are not evaluated and don't even need to be
  valid for the controlling expression's type (as long as they're valid C
  in isolation) - this is what makes it usable as a dispatch macro.
- Exactly one `default:` association is allowed and is optional; if no
  type matches and there's no `default`, it's a compile error - so
  `_Generic` gives you an exhaustiveness check for free if you omit
  `default` and enumerate every type you intend to support.
- The controlling expression undergoes lvalue-to-rvalue conversion and
  array/function-to-pointer decay, but **not** the usual arithmetic
  promotions (integer promotion, float->double) - so `_Generic` sees
  `float` as `float`, not promoted to `double`, and a `char` argument
  matches a `char:` association, not `int:`, even though `char` promotes to
  `int` almost everywhere else in C. This is a common interview gotcha:
  people expect promotion to apply and are surprised when `char` doesn't
  hit their `int:` branch.
- Each type in the association list must be a complete, distinct type
  (no duplicate types, no VLAs) - qualified types like `const int` are
  *not* automatically the same association as `int` per the standard,
  though most compilers strip top-level qualifiers from the controlling
  expression's type before matching in practice; don't rely on that without
  checking your compiler.

## `_Generic` vs. `void *`-based "generic" C

- Pre-C11, "generic" C code (generic containers, generic comparators,
  `qsort`/`bsearch`-style APIs - see `19_QsortBsearch`) worked by type-erasing
  through `void *` plus a size/count or an explicit comparator function
  pointer, and the caller was responsible for casting back to the correct
  type. This dispatches at **runtime** (or rather, avoids dispatch entirely
  by never needing to know the type at all), works across translation
  units and with data whose type genuinely isn't known until runtime, but
  gives up compile-time type checking - a wrong cast is a silent bug, not
  a compile error.
- `_Generic` instead requires knowing the type at the *call site* at
  compile time (it's a macro, expanded per translation unit, per call) -
  it can't express "operate on a type decided at runtime" the way `void *`
  can. What it buys back is compile-time type safety and dispatch: the
  compiler picks the right typed function (or expression) for you, with no
  casts and no way to accidentally call the `float` branch with an `int`.
- In practice they solve different problems: `_Generic` is for "give me one
  macro name that does the right typed thing for whatever type I call it
  with" (ergonomics/type-safety over a small closed set of types known at
  compile time); `void *`-based genericity is for "store/operate on data
  whose type isn't fixed until runtime" (containers, callback registries).
  They're also commonly combined - C11's own `tgmath.h` uses `_Generic`-like
  dispatch to pick the right typed math function, while a generic linked
  list (`29_LinkedList`) still needs `void *` payloads because its element
  type genuinely varies per use.
