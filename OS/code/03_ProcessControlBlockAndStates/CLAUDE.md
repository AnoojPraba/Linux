# 03_ProcessControlBlockAndStates

Conceptual notes on the process control block, process table and the process state diagram (NOTES-only).

## Files
- `NOTES.md` - PCB fields, process table, state diagram, seeing real state via /proc/[pid]/stat, cross-references

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- PCB holds PID, state, registers/PC, memory maps, open files, scheduling info, credentials.
- States: new, ready, running, waiting/blocked, terminated; on Linux `R`, `S`, `D`, `T`, `Z` in `/proc/[pid]/stat` field 3.
- Context switch saves/restores PCB state (see `../27_ContextSwitchMechanics`).
- `D` (uninterruptible) cannot be killed; usually I/O.

## Gotchas
- NOTES-only: nothing to compile; try the `awk` and `ps` commands shown in NOTES.md on this machine.

## Related
- `../04_ProcessLifecycle`
- `../27_ContextSwitchMechanics`
- `../24_ProcessScheduling`
- `../02_Processes`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
