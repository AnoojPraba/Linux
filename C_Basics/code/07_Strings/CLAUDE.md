# 07_Strings

C string basics: null termination, strlen/strcat/strcpy, hand-written string functions and strtok.

## Files
- `01_stringBasics.c` - strlen vs array size, strcpy/strcat into fixed buffers
- `02_manualStringFunctions.c` - own myStrlen, myStrcpy and myStrrev
- `03_tokenizeString.c` - strtok splitting "the,quick,brown,fox"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_stringBasics.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/07_Strings/` (git-ignored).

## Key concepts / interview angles
- strlen counts bytes up to `'\0'`, not the array size.
- strcpy/strcat do no bounds checking; prefer snprintf or sized variants.
- `strtok` mutates its input and keeps hidden static state (not reentrant; use `strtok_r`).
- Reversing in place is a classic two-pointer problem.

## Gotchas
- `03_tokenizeString.c` uses a `char[]` (not a literal) because strtok writes into the buffer.

## Related
- `../03_pointers/08_pointersAndStrings.c`
- `../58_ItoaAtoiSafeParsing`
- `../11_StringPatternMatching`
- `../65_SecurityDemos` - buffer overflows

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
