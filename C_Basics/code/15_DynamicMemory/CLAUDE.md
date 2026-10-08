# 15_DynamicMemory

Heap allocation basics: malloc vs calloc, realloc growth and freeing nested allocations.

## Files
- `01_mallocCalloc.c` - malloc (uninitialised) vs calloc (zeroed)
- `02_realloc.c` - realloc may move the block; always assign to a temp pointer first, never assume the old address is valid
- `03_freeArrayOfStructs.c` - free each struct's owned string, then the array

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_mallocCalloc.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/15_DynamicMemory/` (git-ignored).

## Key concepts / interview angles
- Always check the return of malloc/calloc/realloc.
- `p = realloc(p, n)` leaks the original on failure; use a temp.
- Free in reverse ownership order: members first, then the container.
- `calloc(n, size)` checks the n*size overflow; `malloc(n*size)` does not.

## Gotchas
- Run under `valgrind --leak-check=full` or `-fsanitize=address` to verify no leaks; see `../68_ValgrindAndAsan`.

## Related
- `../68_ValgrindAndAsan`
- `../81_MallocInternalsAndAllocators` - how malloc works
- `../76_ErrorHandlingAndCleanupPatterns`
- `../60_AlignedMallocFree`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
