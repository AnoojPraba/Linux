# 04_array

Array exercises: index tricks, copying, 2D arrays, passing arrays to functions, and true 2D arrays vs arrays of pointers (layout and cache implications).

## Files
- `01_indexremoveproduct.c` - product of all elements except self; O(n^2) method plus the commented alternative
- `02_array_copy.c` - element loop vs memcpy copy
- `03_array_print.c` - reads 5 ints from stdin and prints them
- `04_average.c` - reads n marks from stdin and averages them
- `05_2dArrayVsArrayOfPointers.c` - contiguous row-major int[R][C] vs malloc'd row pointers; prints address strides
- `2d_print.c` - nested loops filling and printing a 2D temperature array
- `parseArrayToFunction.c` - passing int[2][2] to functions (read input, print, increment)
- `sum2array.c` - adds two 2x2 float matrices read from stdin
- `NOTES.md` - 2D array vs array of pointers: layout, cache friendliness, ragged arrays

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_indexremoveproduct.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/04_array/` (git-ignored).

## Key concepts / interview angles
- True 2D array: one contiguous block, `m[i][j]` is `*(m + i*COLS + j)`, one allocation, dims fixed at compile time.
- Array of pointers: separate allocations, extra dereference per access, allows ragged rows and runtime dims.
- Passing a 2D array requires all but the first dimension in the parameter type.
- `memcpy` copy is the clean O(n) alternative to a loop.

## Gotchas
- `03_array_print.c`, `04_average.c`, `2d_print.c`, `parseArrayToFunction.c` and `sum2array.c` read stdin with scanf; pipe input (e.g. `printf "1 2 3 4 5\n" | /tmp/x`) or they will block.

## Related
- `../22_AdvancedArrays` - VLAs and flexible array members
- `../15_DynamicMemory`
- `../03_pointers/07_pointersAndArrays.c` - array decay

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
