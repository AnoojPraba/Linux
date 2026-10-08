# 58_ItoaAtoiSafeParsing

Safe atoi with pre-checked overflow and a fast arbitrary-base itoa, including the INT_MIN trap.

## Files
- `01_safeAtoi.c` - bounded string-to-int that checks overflow before the multiply/add that would overflow
- `02_itoaFast.c` - integer-to-string for bases 2-36, correct for negatives including INT_MIN
- `NOTES.md` - atoi pitfalls, why "multiply then check" is itself UB, the INT_MIN negation trap, efficient itoa

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_safeAtoi.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/58_ItoaAtoiSafeParsing/` (git-ignored).

## Key concepts / interview angles
- `atoi` cannot report errors; prefer `strtol` with errno/endptr checks.
- Check `result > (INT_MAX - digit) / 10` before accumulating; checking after overflow is already UB.
- `-INT_MIN` overflows: work in unsigned magnitude.
- itoa builds digits in reverse then reverses; handle 0, negatives, base range.

## Related
- `../57_UndefinedBehaviorCatalog`
- `../18_CommandLineArgs`
- `../07_Strings`
- `../75_IntegerPromotionsAndConversions`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
