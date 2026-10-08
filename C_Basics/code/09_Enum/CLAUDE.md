# 09_Enum

Enumerations in C: implicit numbering and using an enum to drive a state machine.

## Files
- `01_basicEnum.c` - default values from 0, explicit values, enum as int
- `02_enumStateMachine.c` - enum State with stateName() and nextState() transition function

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_basicEnum.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/09_Enum/` (git-ignored).

## Key concepts / interview angles
- C enums are integer constants with no type safety (any int converts implicitly).
- Enum-driven state machines pair well with a switch or a transition table.
- Add a trailing `COUNT` member for table sizes.

## Related
- `../05_BitManipulation/02_Boolean.c` - enum as boolean
- `../61_FunctionPointersAndCallbacks` - table-driven dispatch
- `../../../OS/code/42_HardwareBuses`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
