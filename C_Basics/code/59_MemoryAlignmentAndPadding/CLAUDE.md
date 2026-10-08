# 59_MemoryAlignmentAndPadding

Natural alignment, struct padding (member ordering changes sizeof) and alignas.

## Files
- `01_structPadding.c` - poor vs good member ordering, printing sizeof and offsets
- `02_alignas.c` - alignas to force stronger (e.g. cache-line) alignment
- `NOTES.md` - alignment rules, padding, packing, cache-line alignment

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_structPadding.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/59_MemoryAlignmentAndPadding/` (git-ignored).

## Key concepts / interview angles
- Each member is aligned to its natural boundary and the struct is padded to a multiple of its largest alignment; order members large-to-small to shrink it.
- `__attribute__((packed))` removes padding but causes misaligned access (slow, or faults on strict-alignment CPUs).
- `alignas(64)` places hot data on its own cache line to avoid false sharing.
- Padding and endianness make raw struct dumps non-portable.

## Related
- `../60_AlignedMallocFree`
- `../../../OS/code/39_FalseSharing`
- `../06_EndiannessAndByteOrder`
- `../08_Structures`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
