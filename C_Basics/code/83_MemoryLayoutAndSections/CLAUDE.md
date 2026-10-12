# 83_MemoryLayoutAndSections

Where each kind of variable lives in a C program (text, data, bss, heap, stack) and what the compiler stages and preprocessor directives do.

## Files
- `01_segmentClassifier.c` - Classify the address of a function, globals, statics, string literal, heap block and local into code/data/bss/heap-or-stack using the linker symbols etext, edata and end (GNU ld).
- `02_preprocessorDirectives.c` - #error, #line, __FILE__/__LINE__/__func__, the stringify-after-expansion trick and a DEBUG switch.
- `03_midLevelLanguage.c` - C as a mid-level language: structured data and control flow next to bit masks, addresses and byte-order inspection.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_segmentClassifier.c -o /tmp/x -lm && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/83_MemoryLayoutAndSections/` (git-ignored).

## Key concepts / interview angles
- **Layout, low to high address:** text (code, read-only) | read-only data (string literals, `const` globals) | data (initialized globals/statics, bytes stored in the file) | bss (zero/uninitialized statics, only a size in the file, loader gives zero pages) | heap (grows up via `brk`/`mmap`) | mmap region (shared libraries) | stack (grows down).
- **Which segment:** locals and parameters: stack. `malloc`: heap. Initialized global/`static`: data. Zero/uninitialized global/`static`: bss. `static` local: data/bss, NOT the stack (that is why it keeps its value). String literal characters: read-only data (writing = undefined behavior, usually SIGSEGV).
- **Why bss is zero but locals are garbage:** static storage is set up by the loader before `main`; a stack slot just holds whatever the last call left there.
- **Inspect:** `size a.out` (text/data/bss bytes), `nm a.out` (`T` text, `D` data, `B` bss, `R` rodata), `readelf -S` / `objdump -h` (sections), `/proc/<pid>/maps` (live regions). Making a zero global initialized moves its bytes from bss to data and grows the file.
- **Linker symbols** `etext`, `edata`, `end` (GNU ld) mark the ends of text, initialized data and bss; the demo prints region names, not addresses, because ASLR randomizes bases.

## Gotchas
- The etext/edata/end symbols are a GNU extension (not portable to MSVC or macOS, where the names differ).
- Everything between etext and edata is reported as "data" and includes the linker's read-only data, so a string literal shows up as data.
