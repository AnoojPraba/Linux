# 10_ThreadingModels

Conceptual notes on user-level vs kernel-level threads and many-to-one, one-to-one, many-to-many mapping (NOTES-only).

## Files
- `NOTES.md` - user vs kernel threads, multithreading models, process-based vs thread-based multitasking, cross-references

## Build and run
- Notes-only folder: nothing to compile.

## Key concepts / interview angles
- One-to-one (Linux NPTL, Windows): true parallelism, blocking syscall blocks one thread only.
- Many-to-one (green threads): cheap but one blocking call stalls all and no multicore use.
- Many-to-many / M:N (Go runtime): scheduler multiplexes goroutines over OS threads.
- Threads share an address space (cheap switch, shared-state bugs); processes isolate (IPC needed).

## Related
- `../09_Threads`
- `../27_ContextSwitchMechanics`
- `../../../C_Basics/code/82_CoroutinesInC`
- `../../../Cpp/code/33_Cpp20Coroutines`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
