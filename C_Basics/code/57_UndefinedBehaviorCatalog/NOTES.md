# Undefined Behavior Catalog

Common C undefined behaviors an interviewer expects you to recognize by name.
All example .c files in this folder show the SAFE way to demonstrate the
concept - they never actually trigger UB, since real UB has no guaranteed
observable effect to demo reliably.

- **Signed integer overflow** (`01_signedOverflow.c`): `INT_MAX + 1` on a
  signed int is UB (unlike unsigned overflow, which is defined to wrap via
  modular arithmetic). Compilers may assume it never happens and optimize
  based on that assumption (e.g. eliminating overflow checks).
- **Strict aliasing violation** (`02_strictAliasing.c`): accessing an object
  through an lvalue of an incompatible type (excluding `char *`) is UB. The
  "type punning" idiom `*(int*)&someFloat` is UB; use `memcpy()` or a union
  (GCC/Clang extension, not portable C) instead.
- **Use-after-free**: dereferencing or freeing a pointer after `free()` has
  already been called on it. The memory may be reused by another allocation,
  silently corrupting unrelated data. See `15_DynamicMemory` for allocation
  lifetime basics.
- **Uninitialized read**: reading an automatic (stack) variable before it is
  assigned. The value is indeterminate - not just "garbage", the compiler is
  free to assume it never happens, which can eliminate branches entirely.
- **Out-of-bounds access**: reading/writing past the end of an array or
  buffer. No bounds checking exists in C; this is the root cause of most
  buffer-overflow security bugs (see `65_SecurityDemos`).
- **Dangling pointer**: a pointer to a stack variable that has gone out of
  scope (e.g. returning `&localVar` from a function), or to freed heap memory.
  The address may still "look" valid but the storage is no longer live.
- General rule: UB means the standard imposes NO requirements on behavior -
  not "crashes", not "does the obvious thing" - genuinely anything, including
  the compiler optimizing away code paths that assume UB can't happen. Always
  compile with `-Wall -Wextra -fsanitize=address,undefined` during
  development to catch these.

## MISRA-C awareness

MISRA-C is a set of coding guidelines - originally created for the
automotive industry, now used broadly across safety-critical/embedded C -
designed to eliminate exactly the kind of undefined-behavior-prone and
ambiguous constructs this folder catalogs above. A few representative
example rules (not exhaustive, and not necessarily verbatim from the real
document, just to give a genuine flavor):

- Ban implicit type conversions that could silently lose data (e.g. an
  `int` narrowed into a `char` without an explicit cast).
- Every `if`/`else if` chain must end with an `else` (even an empty one),
  forcing explicit handling of the "none of the above" case.
- Ban `goto` in most cases, except very restricted forward-jump patterns.
- Every `switch` statement must have a `default` case.
- Restrict or ban dynamic memory allocation after initialization -
  predictable, bounded memory usage matters more than flexibility in
  safety-critical embedded code.
- Ban recursion in many safety-critical contexts, since it produces
  unbounded/hard-to-analyze stack usage (see this repo's `14_Recursion`
  folder for the general technique MISRA-C is wary of here).

Tooling exists to automatically check MISRA-C compliance (static analyzers
configured with a MISRA rule set) - it is not something the compiler
enforces on its own. For a general embedded interview, the expected depth
is simply "know that MISRA-C exists and roughly why" - memorizing specific
rule numbers is only expected when applying for an automotive/safety-critical-
specific role.

## ISO 26262 / ASIL awareness

ISO 26262 is the automotive functional safety standard. It defines a
risk-based classification for automotive electronic/software systems,
assigning each function an **ASIL** (Automotive Safety Integrity Level)
rating based on the severity, exposure, and controllability of a potential
failure:

- **QM** (Quality Managed) - no special safety requirements beyond normal
  quality processes.
- **ASIL A / B / C** - increasing risk, increasing rigor required.
- **ASIL D** - the highest level; failure could cause severe, life-
  threatening injury (e.g. airbag deployment, steering/braking control).

Higher ASIL levels demand progressively more rigorous development
practices:

- More thorough testing/verification (e.g. higher code coverage targets,
  more formal test methods).
- Stricter coding standards - MISRA-C compliance (see the section above)
  becomes effectively mandatory at higher ASILs.
- Redundancy/diverse implementations for critical functions (e.g. two
  independently-implemented algorithms cross-checking each other's result).
- Formal hazard analysis (systematically identifying failure modes and
  their severity/exposure/controllability up front).

This is why ASIL-D-rated code tends to look far more restrictive/verbose
than typical code - defensive checks, explicit error handling on every
path, and no constructs a MISRA/ASIL audit would flag.

As with MISRA-C, the expected interview depth for a general embedded role
is "know it exists, roughly what the ASIL levels represent, and why
higher-ASIL code looks more restrictive" - only automotive-safety-specific
roles would expect deeper knowledge (e.g. actually performing an ASIL
hazard analysis).

For context: ISO 26262 is derived from **IEC 61508**, the parent industrial
functional-safety standard used for non-automotive industrial safety
systems (e.g. PLCs, industrial machinery).
