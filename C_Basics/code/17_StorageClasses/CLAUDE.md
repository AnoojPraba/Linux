# 17_StorageClasses

Storage duration and linkage: static locals, file-scope static, auto and register.

## Files
- `01_staticLocal.c` - static local keeps its value across calls (initialised once)
- `02_staticFileScope.c` - file-scope static gives internal linkage
- `03_registerAuto.c` - auto is the default; register is only a hint

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_staticLocal.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/17_StorageClasses/` (git-ignored).

## Key concepts / interview angles
- `static` has two meanings: persistent storage duration (locals) and internal linkage (file scope).
- Static locals are not thread-safe/reentrant (shared state).
- `register` forbids taking the address and is ignored by modern optimisers.

## Related
- `../30_MultiFile` - extern and linkage across files
- `../80_SignalSafetyAndThreadLocal` - thread-local storage
- `../02_Functions`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
