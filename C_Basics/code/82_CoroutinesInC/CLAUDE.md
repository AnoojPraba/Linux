# 82_CoroutinesInC

Coroutines in C: stackful with ucontext and stackless with the switch/__LINE__ (protothreads) trick, plus a senior Q&A comparing them with state machines.

## Files
- `01_ucontextStackful.c` - POSIX ucontext: each coroutine has its own stack; swapcontext is a user-space context switch
- `02_protothreadsStackless.c` - Duff's-device switch on __LINE__ remembers where to resume; no separate stack
- `NOTES.md` - stackful vs stackless, manual state machines, comparison, "Senior interviewer Q&A"

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_ucontextStackful.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/82_CoroutinesInC/` (git-ignored).

## Key concepts / interview angles
- Stackful: full call stack per coroutine (memory cost, can yield from nested calls); stackless: tiny state, yield only at the top function level, locals must live in a struct.
- swapcontext saves/restores registers (and signal mask via a syscall in glibc), still much cheaper than a thread switch.
- Stackless C coroutines are macro tricks; locals do not survive a yield unless static or in the context.
- Compare with C++20 coroutines (compiler-generated state machine).

## Gotchas
- ucontext is marked obsolescent in POSIX but works in glibc here.

## Related
- `../../../Cpp/code/33_Cpp20Coroutines`
- `../../../OS/code/27_ContextSwitchMechanics`
- `../20_ControlFlowExtras/03_setjmpLongjmp.c`
- `../32_StackAndQueue/08_uartRingBuffer.c`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
