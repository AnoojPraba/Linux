# 68_ValgrindAndAsan

Memory-error detection with valgrind Memcheck and AddressSanitizer using three deliberately broken programs (leak, use-after-free, heap overflow).

## Files
- `01_memoryLeak.c` - allocateBuffer result never freed
- `02_useAfterFree.c` - read through a dangling pointer
- `03_bufferOverflow.c` - heap write past an 8-byte allocation
- `NOTES.md` - Valgrind Memcheck, ASan, when to reach for which, Helgrind, build note

## Build and run
- ASan: `gcc -Wall -Wextra -std=gnu11 -g -fsanitize=address,undefined 02_useAfterFree.c -o /tmp/x && /tmp/x`.
- Valgrind: build with `-g` and run `valgrind --leak-check=full /tmp/x`.
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/68_ValgrindAndAsan/` (git-ignored).

## Key concepts / interview angles
- Valgrind: no recompile, ~20-50x slowdown, precise leak classes (definitely/indirectly/possibly lost).
- ASan: compile-time shadow memory, ~2x slowdown, catches stack and global overflows too.
- Valgrind may miss some heap-overflow patterns that ASan catches.
- Do not run valgrind on an ASan binary.

## Gotchas
- All three programs are intentionally broken; `03_bufferOverflow.c` triggers a GCC `-Wstringop-overflow` warning, which is expected.
- Do not fix the demos.

## Related
- `../15_DynamicMemory`
- `../57_UndefinedBehaviorCatalog`
- `../65_SecurityDemos`
- `../70_CombinedDebuggingCaseStudy`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
