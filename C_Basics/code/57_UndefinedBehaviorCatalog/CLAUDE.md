# 57_UndefinedBehaviorCatalog

Catalog of the undefined behaviours an interviewer expects you to name, plus MISRA-C and ISO 26262/ASIL awareness for safety-critical work.

## Files
- `01_signedOverflow.c` - explains signed overflow (INT_MAX + 1) without triggering it
- `02_strictAliasing.c` - explains the strict-aliasing rule without triggering it
- `NOTES.md` - catalog (signed overflow, aliasing, use-after-free, uninitialised read, out-of-bounds, dangling pointer), MISRA-C awareness, ISO 26262 / ASIL awareness

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_signedOverflow.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/57_UndefinedBehaviorCatalog/` (git-ignored).

## Key concepts / interview angles
- UB means no requirement at all; the compiler may assume it never happens and delete checks.
- Unsigned overflow wraps (defined); signed overflow is UB.
- Type punning: use `memcpy` (or a union in C, not C++), not pointer casts.
- Detect with `-fsanitize=address,undefined`, valgrind, static analysers.
- MISRA-C bans many UB-prone constructs; ASIL levels drive the rigour of verification.

## Gotchas
- By design the examples are safe and never trigger UB; keep it that way (do not "demonstrate" real UB here, results are unreliable).

## Related
- `../62_StrictAliasing`
- `../68_ValgrindAndAsan`
- `../58_ItoaAtoiSafeParsing`
- `../75_IntegerPromotionsAndConversions`
- `../65_SecurityDemos`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
