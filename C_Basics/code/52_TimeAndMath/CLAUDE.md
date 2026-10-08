# 52_TimeAndMath

time.h basics (epoch, localtime/struct tm) and libm math functions (radians, pow/sqrt, NaN handling).

## Files
- `01_timeBasics.c` - time() epoch seconds, localtime() into struct tm, formatting; CPU vs wall time notes
- `02_mathFunctions.c` - sin/cos in radians via M_PI, sqrt, pow, NAN and isnan()

## Build and run
- `02_mathFunctions.c` needs libm: `gcc -Wall -Wextra -std=gnu11 02_mathFunctions.c -o /tmp/x -lm && /tmp/x` (Makefile adds `-lm` via LDLIBS). `-std=gnu11` is needed for `M_PI`; plain `-std=c11` hides it.
- `01_timeBasics.c`: `gcc -Wall -Wextra -std=gnu11 01_timeBasics.c -o /tmp/x && /tmp/x`.
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/52_TimeAndMath/` (git-ignored).

## Key concepts / interview angles
- `time()` returns seconds since the Unix epoch; `localtime` uses the system timezone and returns a pointer to static storage (use `localtime_r` in threads).
- Trig functions take radians; `M_PI` is a POSIX/GNU extension, not ISO C.
- NaN compares unequal to itself; use `isnan()`.
- Wall time vs CPU time: a sleeping process uses ~0 CPU time.

## Gotchas
- Output of 01 depends on the current time and timezone.

## Related
- `../53_FixedPointArithmetic`
- `../../../OS/code/22_SchedulingConceptsDeepDive`
- `../05_BitManipulation`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
