# 79_StackFramesAndCallingConvention

Stack layout, frame pointers and the x86-64 SysV vs AArch64 (AAPCS64) calling conventions, observed with a stack-inspection program.

## Files
- `01_stack_inspect.c` - prints addresses of locals, arguments and frames at -O0 and shows many-argument calls (registers vs stack)
- `NOTES.md` - conventions per architecture, frames, red zone, varargs, stack alignment, "Senior interviewer Q&A"

## Build and run
- Compile with `-O0` so frames are predictable: `gcc -Wall -Wextra -std=gnu11 -O0 -g 01_stack_inspect.c -o /tmp/x && /tmp/x` (the Makefile passes no -O, i.e. -O0).
- `make` from `..` (the parent code/ dir) builds everything into `C_Basics/bin/79_StackFramesAndCallingConvention/` (git-ignored).

## Key concepts / interview angles
- AArch64: args in x0-x7, return in x0, x19-x28 callee-saved, x29 frame pointer, sp 16-byte aligned; x86-64 SysV: rdi, rsi, rdx, rcx, r8, r9.
- The stack grows down on x86 and ARM; frames hold saved registers, locals and return address (lr on AArch64).
- Red zone (x86-64 only) lets leaf functions use memory below rsp.
- Varargs and struct-by-value passing have special ABI rules.
- Frame pointers make profiling and unwinding cheaper.

## Gotchas
- Addresses and layout are ABI/arch/optimisation dependent (this machine is aarch64); do not assert on them.

## Related
- `../67_GdbWorkflow`
- `../14_Recursion`
- `../22_AdvancedArrays`
- `../../../OS/code/27_ContextSwitchMechanics`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.c`); keep each `.c` self-contained and independently compilable (a helper TU without `main()` needs an explicit rule in `../Makefile`).
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`C_Basics/bin/` is git-ignored).
