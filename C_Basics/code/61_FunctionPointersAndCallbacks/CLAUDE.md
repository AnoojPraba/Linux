# 61_FunctionPointersAndCallbacks

Function-pointer idioms: dispatch tables and callback registration (observer-style event source).

## Files
- `01_dispatchTable.c` - table of function pointers invoked in a loop instead of if/else or switch
- `02_callbackRegistration.c` - EventSource with registered listeners and a user-data/context pointer

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_dispatchTable.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/61_FunctionPointersAndCallbacks/` (git-ignored).

## Key concepts / interview angles
- Dispatch tables replace long switch chains and enable data-driven behaviour.
- Callbacks need a `void *userData` context since C has no closures.
- Decide ownership and lifetime of the context; document reentrancy (can a callback unregister itself?).
- Function pointer typedefs make signatures readable.

## Related
- `../55_VTableEmulation`
- `../21_QsortBsearch`
- `../03_pointers/05_functionPointer.c`
- `../80_SignalSafetyAndThreadLocal`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
