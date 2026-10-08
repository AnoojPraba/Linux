# 01_Loops

Warm-up loop exercises: for/while basics, digit reversal and Fibonacci; entry level, here for completeness.

## Files
- `forloop.c` - minimal for loop printing 0..4
- `reverse.c` - reverses the digits of 123456 with a % 10 / / 10 while loop
- `SumFib.c` - iterative Fibonacci up to n=10 using a, b and a temp

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread SumFib.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/01_Loops/` (git-ignored).

## Key concepts / interview angles
- Digit extraction with `% 10` and `/ 10`; accumulate with `ans * 10 + rem`.
- Iterative Fibonacci is O(n) time, O(1) space; contrast with the exponential recursive version in `../14_Recursion/02_fibonacci.c`.
- Watch integer overflow when reversing large numbers (signed overflow is UB, see `../57_UndefinedBehaviorCatalog`).

## Gotchas
- No trailing newlines in the printf calls and `SumFib.c` returns 1 from main; harmless, do not rely on exit codes.

## Related
- `../14_Recursion` - recursive vs iterative forms
- `../57_UndefinedBehaviorCatalog` - signed overflow

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
