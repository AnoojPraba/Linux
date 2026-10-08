# 24_WideChars

wchar_t and wide-string basics, including locale setup for wide output.

## Files
- `01_wcharBasics.c` - wide string L"Hello, ..." with accented characters, setlocale(LC_ALL, "") and wide printf

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_wcharBasics.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/24_WideChars/` (git-ignored).

## Key concepts / interview angles
- `wchar_t` is 32-bit on Linux (UTF-32 code points) but 16-bit on Windows; width is platform dependent.
- Wide output needs `setlocale`; mixing byte and wide I/O on one stream is undefined.
- In practice UTF-8 in plain `char` strings is the portable choice; `strlen` counts bytes, not characters.

## Gotchas
- Output depends on the terminal/locale settings (it calls setlocale(LC_ALL, "")).

## Related
- `../07_Strings`
- `../59_MemoryAlignmentAndPadding`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
