# 65_SecurityDemos

Three classic memory-safety vulnerability patterns with the fix: stack buffer overflow and canary, format string, and integer overflow leading to a heap overflow.

## Files
- `01_bufferOverflowStackCanary.c` - strcpy of a 40-byte string into a 16-byte buffer; compile with -fstack-protector-all to see "stack smashing detected"
- `02_formatStringVulnerability.c` - safeLog (printf("%s", s)) vs vulnerableLog (user string as format)
- `03_integerOverflowToBufferOverflow.c` - count*size wraps before malloc; fix by checking before multiplying

## Build and run
- `gcc -Wall -Wextra -std=gnu11 -fstack-protector-all 01_bufferOverflowStackCanary.c -o /tmp/x && /tmp/x` (expect an abort).
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_bufferOverflowStackCanary.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/65_SecurityDemos/` (git-ignored).

## Key concepts / interview angles
- Stack canary detects but does not prevent the overflow; also know ASLR, NX, RELRO, FORTIFY_SOURCE.
- Never pass user data as a format string; `%n` is a write primitive.
- Check `count > SIZE_MAX / size` before multiplying (or `__builtin_mul_overflow`, `calloc`).
- Prefer `snprintf`/`strlcpy`-style bounded functions.

## Gotchas
- `01_bufferOverflowStackCanary.c` deliberately overflows and will crash (SIGSEGV or abort); that is the demo, do not fix it.
- `02` prints leaked stack values via `%x` (machine-specific).

## Related
- `../57_UndefinedBehaviorCatalog`
- `../68_ValgrindAndAsan`
- `../58_ItoaAtoiSafeParsing`
- `../../../SystemDesign/topics/16_SecurityFundamentalsForInterviews`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
