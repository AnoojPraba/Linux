# 53_FixedPointArithmetic

Q16.16 fixed-point arithmetic for FPU-less embedded targets, and a timing comparison against float.

## Files
- `01_qFormatBasics.c` - double <-> Q16.16 conversion, add/sub, multiply and divide with the rescaling fix
- `02_fixedPointVsFloat.c` - times a Q16.16 integer loop against a float loop using CPU time
- `NOTES.md` - what fixed point is, Q-format, when to use it (no FPU, determinism)

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_qFormatBasics.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/53_FixedPointArithmetic/` (git-ignored).

## Key concepts / interview angles
- Q16.16 = 32-bit signed int with 16 fractional bits (scale 2^16).
- Add/sub need no adjustment; multiply needs a 64-bit intermediate then `>> 16`; divide needs `(a << 16) / b`.
- Watch overflow and rounding/saturation policy; trade range for precision.
- Uses: DSP/control loops on MCUs without FPU, deterministic results across platforms.

## Gotchas
- `02_fixedPointVsFloat.c` prints timings that are machine-specific (aarch64 Raspberry Pi with an FPU will show little or no fixed-point win).

## Related
- `../05_BitManipulation`
- `../75_IntegerPromotionsAndConversions`
- `../52_TimeAndMath`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
