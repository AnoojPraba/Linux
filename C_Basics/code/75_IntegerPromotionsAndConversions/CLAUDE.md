# 75_IntegerPromotionsAndConversions

Usual arithmetic conversions, integer promotion and signed/unsigned traps, with a senior Q&A.

## Files
- `01_promotions.c` - numbered demos: signed/unsigned comparison (-1 becomes UINT_MAX), small-type promotion and similar traps
- `NOTES.md` - promotion and conversion rules plus "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_promotions.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/75_IntegerPromotionsAndConversions/` (git-ignored).

## Key concepts / interview angles
- Types narrower than `int` promote to `int` before arithmetic; mixed signed/unsigned of equal rank converts to unsigned.
- `-1 < 1u` is false; `size_t i >= 0` loops forever; `strlen(s) - strlen(t)` underflows.
- Conversion to a smaller signed type is implementation-defined; signed overflow is UB.
- Shifts: promoted left operand type decides the width; shifting into the sign bit is UB.
- Enable `-Wsign-compare -Wconversion` and use fixed-width types.

## Gotchas
- Several lines print surprising results on purpose; compile with warnings on to see which.

## Related
- `../57_UndefinedBehaviorCatalog`
- `../58_ItoaAtoiSafeParsing`
- `../05_BitManipulation`
- `../02_Functions`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
