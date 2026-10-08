# 21_QsortBsearch

Using the C standard library qsort and bsearch with comparator callbacks.

## Files
- `01_qsortInts.c` - qsort of ints with a const void* comparator
- `02_bsearchStructs.c` - qsort then bsearch on an array of structs by key

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_qsortInts.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/21_QsortBsearch/` (git-ignored).

## Key concepts / interview angles
- Comparator contract: negative / zero / positive; avoid `a - b` (overflow), use `(a > b) - (a < b)`.
- bsearch requires the array sorted with the same comparator.
- qsort is not guaranteed stable and has no context argument (qsort_r is a GNU/POSIX extension).

## Related
- `../12_Sorting`
- `../61_FunctionPointersAndCallbacks`
- `../10_Search_alg`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
