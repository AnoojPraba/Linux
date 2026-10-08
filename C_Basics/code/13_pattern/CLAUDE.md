# 13_pattern

Console pattern-printing exercises (stars/numbers, pyramids and diamonds) built from nested loops; a loop-control warm-up.

## Files
- `Pattern.c` - seven pattern functions (pattern1..pattern7); main() currently calls only pattern7(5)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread Pattern.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/13_pattern/` (git-ignored).

## Key concepts / interview angles
- Derive the number of columns per row from the row index (e.g. `row > n ? 2*n - row : row` for diamond shapes).
- Pattern questions test nested-loop bookkeeping, not algorithms; talk through row/column formulas.

## Gotchas
- Change the call in `main()` to see other patterns; the file is a single translation unit with no input.

## Related
- `../01_Loops`
- `../14_Recursion`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
