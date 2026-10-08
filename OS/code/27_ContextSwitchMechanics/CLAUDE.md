# 27_ContextSwitchMechanics

What a context switch saves and costs, with an empirical sched_yield() timing demo.

## Files
- `01_yieldTiming.c` - times 100000 sched_yield() calls to estimate switch cost
- `NOTES.md` - what is saved/restored, voluntary vs involuntary switches, cost sources, the empirical demo

## Build and run
- Single file: `gcc -Wall -Wextra -std=gnu11 -pthread 01_yieldTiming.c -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `OS/bin/27_ContextSwitchMechanics/` (git-ignored).

## Key concepts / interview angles
- Saved state: registers, PC/SP, FP/SIMD state, page-table base (TLB effects), kernel stack switch.
- Voluntary (blocking, yield) vs involuntary (preemption on quantum expiry, interrupt).
- Direct cost is small; indirect cost (cache/TLB warm-up) dominates; thread switches in one process avoid the address-space switch.
- User-space switches (coroutines, fibers) skip the kernel entirely.

## Gotchas
- Timing results are machine-specific (aarch64 Raspberry Pi here) and noisy; with a single runnable process sched_yield may return without switching.

## Related
- `../03_ProcessControlBlockAndStates`
- `../10_ThreadingModels`
- `../../../C_Basics/code/82_CoroutinesInC`
- `../../../C_Basics/code/79_StackFramesAndCallingConvention`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`OS/bin/` is git-ignored).
