# 43_EmbeddedReliabilityAndPowerManagement

Embedded reliability and power topics (watchdogs, sleep modes, stack limits, debouncing) with a switch-debounce simulation.

## Files
- `01_switchDebounce.c` - simulated noisy button samples filtered by a debounce routine (no real GPIO)
- `NOTES.md` - watchdog timers, low-power modes/sleep states, stack overflow and limited RAM, switch debouncing

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_switchDebounce.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/43_EmbeddedReliabilityAndPowerManagement/` (git-ignored).

## Key concepts / interview angles
- Watchdog: kick it only from a healthy main loop, not from an ISR; windowed watchdogs catch runaway loops.
- Low-power: sleep/deep sleep, wake sources, tickless idle; trade-off latency vs current.
- Stack overflow on small MCUs: static analysis, canary/MPU guard, per-task sizing.
- Debounce: counter/shift-register filter or timer (5-20 ms); also hardware RC.

## Gotchas
- Debounce input is a hard-coded array; no hardware is touched.

## Related
- `../23_RTOSConceptsAndTaskScheduling`
- `../42_HardwareBuses`
- `../44_FirmwareUpdateAndFlashStorage`
- `../../../C_Basics/code/16_ConstVolatile`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
