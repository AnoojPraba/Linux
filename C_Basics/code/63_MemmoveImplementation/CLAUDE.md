# 63_MemmoveImplementation

Hand-written memmove that handles overlapping regions by copying forward or backward, and why memcpy on overlap is UB.

## Files
- `01_myMemmove.c` - direction detection: copy forward if dest < src, backward otherwise
- `NOTES.md` - memmove vs memcpy, why overlap is UB for memcpy, direction detection, real-world optimisations

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_myMemmove.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/63_MemmoveImplementation/` (git-ignored).

## Key concepts / interview angles
- Overlap with dest after src requires copying from the end to avoid overwriting unread bytes.
- `memcpy` has `restrict` params, so overlap is UB; `memmove` is always safe.
- Real libc versions copy word-at-a-time with alignment handling and SIMD.
- Pointer comparison across unrelated objects is itself technically unspecified; use `uintptr_t`.

## Related
- `../23_RestrictQualifier`
- `../62_StrictAliasing`
- `../59_MemoryAlignmentAndPadding`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
