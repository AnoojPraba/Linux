# Coroutines in C

C has no coroutine keyword. Two families to know (C++ equivalent:
`../../../Cpp/code/33_Cpp20Coroutines`):

## Stackful (`01_ucontextStackful.c`)
- Each coroutine has its OWN stack; a switch saves callee-saved registers + SP and
  loads another's (`swapcontext`, or hand-written asm in boost.context/libco/
  libaco; `setjmp/longjmp` tricks are non-portable). They are *fibers*/green
  threads: user-space, cooperative, no preemption.
- Can yield from any call depth. Cost: a stack per coroutine (guard pages, size
  guessing: too small = overflow, too big = wasted VM; `mmap` + `MAP_GROWSDOWN`
  /lazy commit helps), plus switching cost (`swapcontext` also saves/restores the
  signal mask via a syscall - ~100s of ns; asm switch ~10 ns).
- Registers to save follow the ABI (`../79_StackFramesAndCallingConvention`):
  callee-saved registers, SP, return address, FP state.
- Blocking syscalls block the whole OS thread - pair with non-blocking I/O + an
  event loop (`../../../OS/code/67_EpollInDepth`) or one scheduler per core.
- Used by: Go (goroutines with growable stacks), Lua/Ruby fibers, Boost.Context,
  Windows fibers, libtask/libco, many game engines.

## Stackless (`02_protothreadsStackless.c`)
- The coroutine is a function + a small state struct; yield = `return` after
  recording a resume point, resume = `switch` to it. Variables that must
  survive a yield must live in the struct (stack unwound).
- Zero extra stack, tiny RAM: ideal for MCUs (Contiki protothreads, many RTOS
  state machines, protocol parsers/FSMs). Compiler-assisted equivalents: C++20
  coroutines, Rust `async`, Python generators, C# `async`.
- Restrictions: no yield from nested helper calls; careful with `switch` inside the
  coroutine; debugging is awkward; macros hide control flow.

## State machines - the manual alternative
Explicit `enum state` + `switch` in an event handler: more verbose but debuggable,
testable, and what most firmware uses (`../../../OS/code/75_UartSerialProgramming`
frame parser is one).

## Comparison
| | Threads | Stackful coroutines | Stackless coroutines |
|---|---|---|---|
| Scheduling | preemptive (kernel) | cooperative | cooperative |
| Memory each | MBs (virtual) | KBs-MBs | bytes-KBs |
| Switch cost | us (syscall) | 10-500 ns | ~function call |
| Parallelism | yes | only if multiple carrier threads | same |
| Yield depth | any | any | only in the coroutine itself |

## Senior interviewer Q&A
**Q: How would you implement coroutines in C?**
A: Stackful: allocate a stack, `getcontext/makecontext/swapcontext` (or asm), keep a
run queue. Stackless: a state variable + `switch(state)` re-entry (Duff's device)
with all persistent locals stored in a struct. Mention the trade-offs above.

**Q: Why do locals not survive a `CR_YIELD`?**
A: The function really returns, so its stack frame is destroyed. A stackless
coroutine therefore keeps state in a heap/static/caller-owned struct; compilers
that implement `async` do this automatically by hoisting live variables into the
coroutine frame.

**Q: What is the danger of blocking calls in coroutine-based servers?**
A: A blocking `read`/`sleep`/DB call stalls the whole thread's scheduler and all
coroutines on it. Use non-blocking I/O with an event loop or hand blocking work
to a thread pool.

**Q: Stackful vs stackless - which would you choose for 100k concurrent
connections?**
A: Stackless (or small-stack growable fibers): memory per task matters more than
call-depth flexibility; 100k x 64 KB stacks = 6.4 GB vs a few MB of frames.
Go gets by with growable 2-8 KB stacks.

**Q: How is a context switch implemented?**
A: Save callee-saved registers and the stack pointer of the current context into
its control block, load the next context's, and `ret` into its saved instruction
pointer - exactly what a kernel context switch does in user space, minus the
privilege change and TLB/page-table switch.
