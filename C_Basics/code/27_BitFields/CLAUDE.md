# 27_BitFields

Struct bit-fields: packing several small fields into a word, mirroring hardware registers or flags.

## Files
- `01_bitFieldStruct.c` - struct with N-bit members packed together; prints sizeof and field values

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_bitFieldStruct.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/27_BitFields/` (git-ignored).

## Key concepts / interview angles
- Bit-field layout (order, straddling storage units, signedness of plain `int` fields) is implementation-defined; do not use for on-the-wire formats.
- You cannot take the address of a bit-field.
- For portable register access prefer shifts and masks.

## Related
- `../05_BitManipulation`
- `../16_ConstVolatile`
- `../59_MemoryAlignmentAndPadding`
- `../06_EndiannessAndByteOrder`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
