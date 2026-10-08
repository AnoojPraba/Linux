# 18_CommandLineArgs

argc/argv basics.

## Files
- `01_argcArgv.c` - prints argc and each argv entry; argv[0] is the program name

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_argcArgv.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/18_CommandLineArgs/` (git-ignored).

## Key concepts / interview angles
- `argv[argc]` is NULL; arguments are strings, convert with `strtol` (not `atoi`) to detect errors.

## Related
- `../26_CommandLineOptions` - getopt parsing
- `../58_ItoaAtoiSafeParsing`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
