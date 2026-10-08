# 22_AdvancedArrays

C99 variable-length arrays and flexible array members.

## Files
- `01_variableLengthArray.c` - VLA with runtime size, allocated on the stack, no failure check possible
- `02_flexibleArrayMember.c` - struct with a trailing unsized array; one malloc holds header and payload

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_variableLengthArray.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/22_AdvancedArrays/` (git-ignored).

## Key concepts / interview angles
- VLAs live on the stack: a large or attacker-controlled size overflows it, and they are optional in C11.
- A flexible array member must be last and is not counted in `sizeof(struct)`; allocate `sizeof(struct) + n*sizeof(elem)`.
- Flexible array members enable one-allocation variable-size packets/buffers.

## Related
- `../04_array/05_2dArrayVsArrayOfPointers.c`
- `../59_MemoryAlignmentAndPadding`
- `../15_DynamicMemory`
- `../79_StackFramesAndCallingConvention`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
