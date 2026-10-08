# 26_CommandLineOptions

Option parsing with POSIX getopt.

## Files
- `01_getoptBasics.c` - getopt with optstring "vo:" (flag -v, option -o taking an argument), optind for remaining args

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_getoptBasics.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/26_CommandLineOptions/` (git-ignored).

## Key concepts / interview angles
- A colon after a letter marks a required argument; `optarg` holds it and `optind` is the first non-option argument.
- Handle the `?` return for unknown options; `getopt_long` adds `--long` options.

## Gotchas
- Try `/tmp/x -v -o out file1`; it prints verbose, outputFile and optind (no args: verbose = 0, outputFile = (none)).

## Related
- `../18_CommandLineArgs`
- `../58_ItoaAtoiSafeParsing`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
