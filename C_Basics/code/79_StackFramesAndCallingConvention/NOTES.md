# Stack Frames and Calling Conventions

- **Calling convention (ABI)** = who puts arguments where, who saves which
  registers, who cleans the stack. It is why separately compiled objects and
  libraries (and C <-> asm <-> other languages) interoperate.
- **x86-64 System V (Linux/macOS):** integer/pointer args in `rdi, rsi, rdx,
  rcx, r8, r9`; floats in `xmm0-7`; the rest on the stack, right to left;
  return in `rax` (`rdx:rax` for 128-bit), `xmm0` for floats. Callee-saved:
  `rbx, rbp, r12-r15`. Caller-saved (scratch): the rest. 128-byte **red zone**
  below `rsp` that leaf functions may use without adjusting `rsp` (why kernel
  code is built `-mno-red-zone`: interrupts would clobber it).
- **AArch64 (AAPCS64, e.g. Raspberry Pi):** args in `x0-x7`, floats in
  `v0-v7`, return in `x0`/`v0`; `x19-x28` callee-saved; `x29` = frame pointer,
  `x30` = link register (return address lives in a register, saved to the
  stack by non-leaf functions); `sp` 16-byte aligned. Large struct returns use
  the pointer in `x8`.
- **Windows x64:** `rcx, rdx, r8, r9` + 32-byte shadow space - different from SysV.
- **Frame layout (typical, `-O0`):** `[args beyond registers] [return addr]
  [saved fp] [callee-saved regs] [locals] [outgoing args]`. Stack grows toward
  lower addresses on x86 and ARM; `sp` 16-byte aligned at a call boundary.
- **Prologue/epilogue:** push/save frame pointer, set new fp, `sub sp`;
  reversed on exit. `-fomit-frame-pointer` (default at `-O2`) frees a register
  but makes unwinding rely on DWARF `.eh_frame` data; profilers and tracers
  prefer `-fno-omit-frame-pointer`.
- **Why stack buffer overflows work:** an overflowing local array overwrites
  saved fp / return address. Mitigations: stack canaries
  (`-fstack-protector-strong`), non-executable stack (NX), ASLR, PIE, shadow
  stacks / pointer authentication (CET, PAC), `-D_FORTIFY_SOURCE`.
  See `../65_SecurityDemos`.
- **Returning a pointer to a local** = dangling pointer: the frame is reused
  by the next call.
- **Variadic functions** need the caller to tell the callee how many args
  (format string); on x86-64 `al` holds the number of vector registers used.
  Passing the wrong type to `va_arg` is UB (`../19_Variadic`).
- **Tail-call optimization:** `-O2` may reuse the frame for `return f(x);` -
  makes deep tail recursion constant-space, but the C standard does not
  guarantee it.
- **Stack size:** default 8 MB per main thread (`ulimit -s`), ~8 MB (glibc,
  `RLIMIT_STACK`) or 2 MB for new pthreads; overflow hits a guard page ->
  SIGSEGV. Deep recursion and big local arrays/VLAs are the usual cause.
- **Struct passing:** small structs may be split across registers; large ones
  are copied to the stack or returned via a hidden pointer (sret). Passing big
  structs by value is costly - pass by `const` pointer.
- **Inspecting it:** `gcc -S -O0`, `objdump -d`, gdb `bt`, `info frame`,
  `x/16gx $sp`; `__builtin_frame_address(0)`, `__builtin_return_address(0)`
  (demo in `01_stack_inspect.c`). Debugging: `../67_GdbWorkflow`.
- Related: `../55_VTableEmulation`, `../../../OS/code/27_ContextSwitchMechanics`
  (which registers a context switch must save follows from this ABI).

## Senior interviewer Q&A
**Q: Walk me through what happens on the stack when `f(a, b)` is called.**
A: Caller places the first args in registers per the ABI (stack for extras),
`call` pushes the return address (x86-64) or writes it to `x30` (AArch64);
callee prologue saves the old frame pointer and any callee-saved registers
it uses and reserves space for locals; epilogue reverses it and `ret` jumps
back. Return value comes back in `rax`/`x0`.

**Q: Which registers must a callee preserve?**
A: SysV x86-64: `rbx, rbp, r12-r15` (and `rsp`). AAPCS64: `x19-x28`, `x29`,
`sp` (and low halves of `v8-v15`). Everything else is caller-saved scratch.
This determines what a context switch or coroutine switch must save
(`../../../OS/code/27_ContextSwitchMechanics`).

**Q: How does a stack buffer overflow become code execution, and what stops it?**
A: Overwriting the saved return address redirects `ret`. Mitigations:
stack canaries (`-fstack-protector-strong`), NX stack, ASLR/PIE,
`_FORTIFY_SOURCE`, CET shadow stack / ARM PAC+BTI. Bypasses (ROP, leaks)
exist, so bounds-checked code remains the real fix. See `../65_SecurityDemos`.

**Q: Why do profilers want `-fno-omit-frame-pointer`?**
A: With frame pointers, unwinding is a cheap linked-list walk. Without them,
tools need DWARF CFI (`.eh_frame`) unwinding, which is heavier and often
unavailable in sampling contexts like `perf` or eBPF. The cost is one fewer
general register.

**Q: Why can't you return a pointer to a local array?**
A: The frame is released on return; the next call reuses the memory, so the
pointer reads garbage or corrupts live data. Return heap memory, a caller
buffer, or a `static` (not thread-safe).

**Q: What causes a stack overflow, and how do you diagnose it?**
A: Unbounded or deep recursion, large locals/VLAs/`alloca`, thread stacks
sized too small. It faults on the guard page -> SIGSEGV. Diagnose in gdb
(huge `bt`, `$sp` near the guard), `ulimit -s`, `-fstack-usage` /
`-Wstack-usage=N`, AddressSanitizer's stack-overflow report. Fixes: iterate,
heap-allocate, raise `pthread_attr_setstacksize`.

**Q: How do variadic functions work at the ABI level?**
A: Named args follow the normal convention; the callee spills the register
args to a save area and `va_arg` walks it, then the stack overflow area. On
x86-64 the caller sets `al` to the number of vector registers used. The callee
cannot know the types - format strings or sentinels convey them; a mismatch is
UB.

**Q: Does the compiler guarantee tail-call optimization?**
A: No. GCC/Clang usually do it at `-O2` for simple cases, but C offers no
guarantee (and debug builds don't). Do not rely on it for correctness in
deeply recursive code.

**Q: Large struct passed or returned by value - what is the cost?**
A: Copy to the stack (or a hidden `sret` pointer for returns). Pass `const T *`
unless the struct is tiny (<=16 bytes often travels in registers).
