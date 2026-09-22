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
  silently corrupting unrelated data. See `13_DynamicMemory` for allocation
  lifetime basics.
- **Uninitialized read**: reading an automatic (stack) variable before it is
  assigned. The value is indeterminate - not just "garbage", the compiler is
  free to assume it never happens, which can eliminate branches entirely.
- **Out-of-bounds access**: reading/writing past the end of an array or
  buffer. No bounds checking exists in C; this is the root cause of most
  buffer-overflow security bugs (see `77_SecurityDemos`).
- **Dangling pointer**: a pointer to a stack variable that has gone out of
  scope (e.g. returning `&localVar` from a function), or to freed heap memory.
  The address may still "look" valid but the storage is no longer live.
- General rule: UB means the standard imposes NO requirements on behavior -
  not "crashes", not "does the obvious thing" - genuinely anything, including
  the compiler optimizing away code paths that assume UB can't happen. Always
  compile with `-Wall -Wextra -fsanitize=address,undefined` during
  development to catch these.
