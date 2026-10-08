# 66_ThreadSanitizerDemo

A deliberately racy shared counter that ThreadSanitizer reliably detects.

## Files
- `01_racyCounter.c` - 4 threads x 100000 unsynchronised `counter++`; comments point to the mutex and atomic fixes in OS/

## Build and run
- `gcc -Wall -Wextra -std=gnu11 -g -fsanitize=thread -pthread 01_racyCounter.c -o /tmp/x && /tmp/x` (prints a TSan data race report).
- Without `-fsanitize=thread` it just prints a (usually too small) total; `make` builds it without TSan.
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/66_ThreadSanitizerDemo/` (git-ignored).

## Key concepts / interview angles
- `counter++` is load/add/store: lost updates are the symptom of the race.
- TSan instruments memory accesses and reports races with both stack traces; slowdown ~5-15x.
- Fixes: mutex, or `_Atomic int`/`atomic_fetch_add` (see OS/code/09_Threads and 20_Atomics).
- TSan cannot be combined with ASan in the same binary.

## Gotchas
- The race is intentional; do not fix it in this file, it is the TSan target.

## Related
- `../../../OS/code/09_Threads`
- `../../../OS/code/20_Atomics`
- `../70_CombinedDebuggingCaseStudy`
- `../68_ValgrindAndAsan`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
