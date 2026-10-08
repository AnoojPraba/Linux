# 56_UnitTesting

Minimal assert-style unit testing in C: test macros, counters and a CI-friendly exit code.

## Files
- `01_assertBasedTests.c` - hand-rolled CHECK-style tests, pass/fail counts, returns nonzero on failure

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_assertBasedTests.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/56_UnitTesting/` (git-ignored).

## Key concepts / interview angles
- Frameworks (CUnit, Check, Unity, cmocka) add discovery, fixtures and reporting on this same core.
- A nonzero exit status makes failing tests fail the CI build.
- Prefer plain-return tests over `assert` (NDEBUG disables assert) so tests still run in release builds.
- Testable design: dependency injection via function pointers for I/O and time.

## Related
- `../20_ControlFlowExtras/02_assert.c`
- `../71_CMakeIntroduction` - ctest integration
- `../61_FunctionPointersAndCallbacks`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
