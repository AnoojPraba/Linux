# 03_pointers

Pointer fundamentals through eight small programs: address/dereference, arithmetic, double pointers, function pointers, void*, array decay and char array vs string literal.

## Files
- `01_changeValueWithPointers.c` - `int* pc, c;` declaration pitfall; changing a value via an alias
- `02_pointers.c` - address-of, dereference and printing addresses
- `03_pointerArithmetic.c` - p + i steps in units of sizeof(*p) over an int array
- `04_doublePointer.c` - `int **out` to return a malloc'd buffer from a function
- `05_functionPointer.c` - function pointers and an array of function pointers as a dispatch table
- `06_voidPointer.c` - void* must be cast before dereference (printAsInt/printAsFloat)
- `07_pointersAndArrays.c` - array parameter decays to pointer, so sizeof(arr) inside the callee is sizeof(int*)
- `08_pointersAndStrings.c` - `char arr[]` (modifiable copy) vs `char *str = "..."` (literal, writes are UB)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_changeValueWithPointers.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/03_pointers/` (git-ignored).

## Key concepts / interview angles
- In `int* pc, c;` only `pc` is a pointer.
- Pointer arithmetic scales by element size; `a[i]` is `*(a + i)`.
- Array-to-pointer decay loses the length; pass a size explicitly.
- Double pointer lets a callee allocate and hand back memory.
- Writing to a string literal is UB; use `char[]` when you need to mutate.

## Gotchas
- `08_pointersAndStrings.c` deliberately avoids writing to the string literal (only `arr[0] = 'H'` on the array); do not add `*str = ...`, it is UB.
- Printed addresses vary run to run (ASLR).

## Related
- `../15_DynamicMemory` - malloc/free used with double pointers
- `../61_FunctionPointersAndCallbacks` - deeper callback patterns
- `../62_StrictAliasing` - pointer casting rules
- `../07_Strings`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
