Interview questions a ~13-year C developer should expect, beyond basic
syntax, with answers. For the "war story" questions, the answer given is
the shape a good answer takes - the interviewer wants your specific
example in that shape, not this generic version verbatim. Pointers to
where this repo already demonstrates the underlying mechanism are
included where relevant.

===========================================================================
Memory and performance debugging
===========================================================================

Q: How would you debug a memory leak / use-after-free / heap corruption
   in production?
A: Reproduce locally under Valgrind (`valgrind --leak-check=full`) or
   AddressSanitizer (`-fsanitize=address`) first - ASan is fast enough to
   run in CI and catches use-after-free/heap-buffer-overflow at the exact
   faulting instruction, with a stack trace for both the bad access and
   the original allocation. If it only reproduces in production, capture
   a core dump (`ulimit -c unlimited`, or a crash handler that calls
   `abort()` after logging) and load it in gdb (`gdb -c core ./binary`) to
   walk the stack at the moment of the crash. For leaks specifically,
   Valgrind's leak summary groups by allocation stack trace, which
   usually points straight at the missing `free()`.

Q: Why does struct field ordering affect its size?
A: The compiler pads each member to satisfy its own alignment requirement
   (an 8-byte `double` can't start at an odd offset, for example), and
   pads the whole struct's total size up to a multiple of its strictest
   member's alignment. Putting large/aligned members first and small ones
   (like `char`, `_Bool`) last minimizes wasted padding - reordering
   fields alone can shrink a struct with no logic change. See
   `07_Structures/` for the struct basics this builds on.

Q: What is cache locality / false sharing, and why does it matter?
A: Cache locality: accessing memory that's physically close together
   (e.g. walking an array sequentially) is much faster than scattered
   access, because the CPU pulls in a whole cache line (typically 64
   bytes) at a time. False sharing: two threads writing to *different*
   variables that happen to land in the same cache line will still
   invalidate each other's cache line on every write, causing real
   slowdown with zero actual logical contention - contrast with the
   genuine contention in `49_Threads/02_mutex.c`, where the slowdown is
   from correctness-required locking, not an accidental layout collision.
   The fix is padding/aligning per-thread data so each thread's hot
   variables land on separate cache lines.

Q: Stack vs heap tradeoffs, and how do you diagnose a stack overflow?
A: Stack allocations are fast (just a pointer bump) and freed
   automatically on scope exit, but are limited in size (often ~8 MB per
   thread by default on Linux) and don't survive the function returning.
   Heap allocations survive beyond the allocating function and can be
   much larger, at the cost of explicit `malloc`/`free` bookkeeping and
   slower allocation. A stack overflow crash typically shows as SIGSEGV
   with a faulting address just past the stack's guard page; a core dump
   or `ulimit -s` check, plus looking for unbounded recursion or a huge
   local array/VLA (see `20_AdvancedArrays/01_variableLengthArray.c`) is
   the usual root cause.

===========================================================================
Concurrency depth
===========================================================================

Q: Why isn't `volatile` a substitute for atomics or a mutex?
A: `volatile` only tells the compiler "don't cache this in a register,
   always re-read/re-write memory" (see
   `14_ConstVolatile/02_volatile.c`) - it says nothing about memory
   ordering or visibility across CPU cores, and it doesn't make an
   operation like `counter++` atomic (that's still a separate read,
   increment, and write, which two threads can interleave). Real
   thread-safety needs either a mutex
   (`49_Threads/02_mutex.c`) or a genuinely atomic type
   (`52_Atomics/01_stdatomicCounter.c`), which provide the actual
   indivisibility and ordering guarantees `volatile` doesn't.

Q: Lock-free vs lock-based - when is each worth it?
A: Lock-based code (mutexes, condition variables) is far easier to reason
   about and debug, and is the right default. Lock-free structures avoid
   the cost of blocking/context-switching and can't deadlock, but are
   substantially harder to get correct (subtle memory-ordering bugs that
   only show up under real contention) and usually only pay off in a
   proven hot path with measured contention - reach for a profiler before
   reaching for lock-free code, not instinct.

Q: What is the ABA problem?
A: In a lock-free algorithm using compare-and-swap, a thread reads a
   value A, gets preempted, another thread changes it to B and back to A,
   and the first thread's CAS succeeds because the value matches again -
   even though the underlying state changed in between. Common fix:
   tag every value with a version/counter so "the same value" can be
   distinguished from "the same value, but it's a different generation".

Q: Why do `49_Threads/03_conditionVariable.c` and
   `50_SyncProblems/01_boundedBufferProducerConsumer.c` re-check their
   condition in a `while` loop instead of an `if` after
   `pthread_cond_wait`?
A: `pthread_cond_wait` can return even without a real `signal`/`broadcast`
   ("spurious wakeup", allowed by POSIX for implementation reasons), and
   even a real signal can wake more waiters than intended if multiple
   threads are waiting on the same condition. Re-checking the actual
   condition in a loop makes the code correct regardless of why it woke
   up, instead of trusting that a wakeup always means "the condition is
   now true".

Q: Deadlock vs starvation vs livelock - what's the difference?
A: Deadlock: two or more threads are each waiting on a resource the other
   holds, and neither can ever proceed (see
   `49_Threads/05_deadlockAvoidance.c`,
   `50_SyncProblems/03_diningPhilosophers.c`). Starvation: a thread *can*
   proceed in principle but never actually gets scheduled/granted the
   resource because others keep getting priority (see the aging note in
   `Notes/23_CPUSchedulingAlgorithms.c`). Livelock: threads are actively
   running and changing state, but never make real progress - e.g. two
   threads each repeatedly backing off and retrying in a way that keeps
   colliding, unlike deadlock where they're simply blocked and idle.

Q: What is priority inversion?
A: A low-priority thread holds a lock; a high-priority thread blocks
   waiting for that same lock; meanwhile a medium-priority thread
   preempts the low-priority one, so the high-priority thread is
   effectively stuck waiting behind a lower-priority thread it should
   never have had to wait on. Priority-inheritance mutexes fix this by
   temporarily boosting the lock holder's priority to match the highest
   priority thread waiting on it.

Q: Describe a real production race condition you've fixed.
A: (Answer in this shape, with your own specifics: what the symptom was -
   usually an intermittent, hard-to-reproduce crash or wrong value; how
   you narrowed it down - often a thread sanitizer, careful log
   timestamps, or code review of anything touching shared state without a
   lock; what the actual interleaving was; and the fix - usually adding
   or correcting the scope of a lock, or switching a shared flag to an
   atomic.)

===========================================================================
Systems-level design
===========================================================================

Q: How would you design a thread pool?
A: A fixed set of worker threads, each looping: lock a shared task queue,
   wait on a condition variable while it's empty (same pattern as
   `50_SyncProblems/01_boundedBufferProducerConsumer.c`), pop a task,
   unlock, run the task outside the lock, repeat. Submitting a task locks
   the queue, pushes the task, and signals the condition variable. Key
   design questions to raise unprompted: bounded vs unbounded queue,
   graceful shutdown (a poison-pill task or a shutdown flag checked under
   the same lock), and whether tasks can themselves submit more tasks.

Q: How would you design a lock-free SPSC (single-producer,
   single-consumer) ring buffer?
A: A fixed-size array plus a head and tail index, each written by only
   one side (producer writes tail, consumer writes head) - because each
   index has exactly one writer, plain atomic loads/stores
   (`52_Atomics/01_stdatomicCounter.c`) are enough, no CAS loop needed.
   The producer checks `(tail + 1) % size != head` before writing (buffer
   not full); the consumer checks `head != tail` before reading (buffer
   not empty). This only works safely because it's restricted to exactly
   one producer and one consumer - MPMC (multi-producer/multi-consumer)
   needs a genuinely lock-free CAS-based design instead.

Q: How would you design memory layout for an embedded system with a
   fixed RAM budget?
A: Prefer static/global allocation and fixed-size pools over dynamic
   `malloc`/`free` where possible, to make worst-case memory usage
   provable at compile time rather than dependent on runtime
   fragmentation. Use bit-fields (`25_BitFields/01_bitFieldStruct.c`) and
   packed structs where every byte counts, and be deliberate about
   stack usage per task/thread when the platform has small, fixed
   per-task stacks.

Q: What endianness issues come up in network-facing C code, and how do
   you handle them?
A: Different architectures store a multi-byte value's bytes in different
   orders (little-endian vs big-endian); a value written on one machine
   and read raw on another can come out completely wrong. Network
   protocols fix a "network byte order" (big-endian), and `htons`/`htonl`/
   `ntohs`/`ntohl` convert between host and network order - see where
   this would actually matter in `69_SocketProgramming/01_tcpServer.c`
   (`htons(SERVER_PORT)`).

Q: Why can changing a struct's fields break a shared library's callers
   without a recompile, even if the source still compiles fine?
A: If a caller was compiled against an older struct layout (different
   field order, size, or offsets) and the shared library `.so` is updated
   independently, the caller's binary still uses the *old* offsets to
   access fields - it never recompiled, so it has no idea the layout
   changed. This is why stable ABIs either freeze public struct layouts,
   only append new fields at the end, or hide the struct behind an opaque
   pointer and accessor functions instead of exposing fields directly.

===========================================================================
Undefined behavior and standards knowledge
===========================================================================

Q: Is signed integer overflow defined behavior in C?
A: No - signed overflow is undefined behavior, while unsigned overflow is
   well-defined to wrap modulo 2^n (see `02_int_keyword.c`). A compiler is
   free to assume signed overflow never happens and optimize accordingly,
   which is exactly why code that "worked" by accident with overflowing
   signed math can break under a different optimization level.

Q: What is a strict aliasing violation?
A: Accessing the same memory through two pointers of unrelated types
   (e.g. writing through a `float *` and reading the same bytes through
   an `int *`) violates the strict aliasing rule, letting the compiler
   assume those two pointers can never refer to the same object and
   reorder/optimize accesses accordingly - producing wrong results if the
   assumption is actually violated. `-fno-strict-aliasing` is a common
   real-world workaround/escape hatch for legacy code that relies on this
   pattern instead of using a union or `memcpy` to reinterpret bytes
   correctly.

Q: Why can `-O2` "break" code that worked fine at `-O0`?
A: Almost always latent undefined behavior (signed overflow, strict
   aliasing violation, use of an uninitialized value, out-of-bounds
   access) that happened to produce the "expected" result at `-O0`
   because the compiler generated straightforward, unoptimized code - at
   `-O2` the optimizer is allowed to assume UB never happens and
   transforms the code in ways that expose it. It is essentially never a
   compiler bug; the fix is finding and removing the UB, not avoiding
   optimization.

Q: What practical differences between C89/C99/C11 matter for legacy or
   embedded work?
A: C89 requires all declarations at the top of a block (this repo's own
   coding convention, per CLAUDE.md, even on newer standards) and has no
   `//` comments, `_Bool`, or variable-length arrays. C99 added VLAs
   (`20_AdvancedArrays/01_variableLengthArray.c`), designated
   initializers, and `_Bool`. C11 added `_Generic`
   (`28_Generics/01_genericMacro.c`), `_Thread_local`, and
   `<stdatomic.h>` (`52_Atomics/`). Embedded toolchains often lag several
   standards behind, so knowing which features are actually available on
   a given target's compiler matters more than knowing the newest
   standard exists.

===========================================================================
Build and tooling maturity
===========================================================================

Q: What static analysis tools have you used, and how do you handle false
   positives?
A: Coverity, cppcheck, and clang-tidy are the common ones; clang-tidy
   also doubles as a style/modernization linter. False positives get
   triaged by understanding *why* the tool flagged it (usually it can't
   see an invariant that's actually true, e.g. a null check performed in
   a different function) and either restructuring the code to make the
   invariant locally visible, or suppressing that specific finding with a
   documented inline comment explaining why - never a blanket suppression
   of the whole check.

Q: What does cross-compilation and linker script experience look like?
A: Cross-compiling means building for a target architecture different
   from the build machine (e.g. building ARM binaries on an x86 CI
   runner) using a cross-toolchain (`arm-none-eabi-gcc` etc.) - the key
   pitfalls are accidentally linking against host libraries instead of
   target ones, and endianness/word-size mismatches. A linker script
   controls how sections (`.text`, `.data`, `.bss`) are placed in memory,
   which matters directly for embedded targets with fixed memory regions
   (flash vs RAM) - reading a `.map` file (which shows exactly where each
   symbol landed and how large each section is) is the usual way to debug
   a linker script or diagnose "why is my binary too big for flash".

Q: What gdb techniques go beyond basic breakpoints?
A: Watchpoints (`watch <expr>`) break when a value changes, not at a
   fixed line - useful for "something is corrupting this variable, but I
   don't know where". Conditional breakpoints (`break foo if x == 5`)
   avoid manually stepping through many irrelevant hits. Reverse debugging
   (`record`/`reverse-next`, where supported) lets you step *backward*
   from a crash to find where a bad value was actually introduced. Core
   dump post-mortem analysis (`gdb -c core ./binary`, `bt` for a
   backtrace) is the standard way to debug a crash that already happened
   in production without being able to reproduce it live.

Q: What tools trace syscalls/library calls or profile CPU usage, beyond gdb?
A: strace intercepts and logs every syscall a process makes (great for
   "why does this hang on startup" - often a missing file or blocked
   syscall becomes obvious immediately). ltrace does the same for
   dynamic library calls. perf samples where a running program's time is
   actually spent (`perf record`/`perf report`, or rendered as a flame
   graph) - the standard first step for "this is slow, where" instead of
   guessing.

===========================================================================
I/O multiplexing and networking depth
===========================================================================

Q: What is the difference between select, poll, and epoll?
A: select() takes a fixed-size bitmask of file descriptors to watch
   (capped at FD_SETSIZE, typically 1024) and requires rebuilding that
   mask and re-describing the whole watch set on every call. poll() takes
   an array of {fd, events} structs instead, removing the fixed-size cap
   but still requiring the whole set to be passed and rescanned every
   call. epoll (Linux-specific) keeps the watch list inside a kernel
   object across calls via epoll_ctl(), so epoll_wait() only has to report
   which already-registered descriptors are ready - the difference barely
   matters for a handful of descriptors, but matters enormously at
   thousands of concurrent connections, which is why high-performance
   servers use epoll. See `code/71_IOMultiplexing/01_selectMultiplePipes.c`,
   `02_pollMultiplePipes.c`, and `03_epollServer.c`, all watching the same
   two pipes with each mechanism.

Q: TCP vs UDP - what are the actual tradeoffs?
A: TCP (SOCK_STREAM, `69_SocketProgramming/`) guarantees ordered, reliable
   delivery via connection setup, acknowledgments, and retransmission, at
   the cost of that setup latency and per-connection state. UDP
   (SOCK_DGRAM, `code/70_UDPSockets/`) has no connection, no ordering or
   delivery guarantee, and no accept() - every packet is independently
   addressed via sendto()/recvfrom() - but has far lower overhead, which
   is why it fits latency-sensitive use cases (live video/audio, some game
   state) where an occasional dropped packet is acceptable and the
   application can handle its own retries if it needs any.

Q: What is Nagle's algorithm, and why do people disable it?
A: Nagle's algorithm delays sending small TCP packets briefly, batching
   them together to avoid flooding the network with tiny packets - great
   for bulk transfer, but it can add noticeable latency to
   request/response protocols expecting a fast round trip, which is why
   latency-sensitive services often disable it with TCP_NODELAY.

Q: SO_REUSEADDR vs SO_REUSEPORT?
A: SO_REUSEADDR (used in `69_SocketProgramming/01_tcpServer.c`) lets a
   socket rebind to a port still lingering in TIME_WAIT from this same
   program's previous run, avoiding "address already in use" on a quick
   restart. SO_REUSEPORT goes further, letting multiple independent
   sockets (often one per worker process/thread) all bind to the exact
   same port simultaneously, with the kernel load-balancing incoming
   connections across them - a common pattern for multi-process servers
   that want to avoid a single shared listening socket becoming a
   bottleneck.

===========================================================================
Security-adjacent
===========================================================================

Q: How does a classic stack buffer overflow work, and how do canaries
   mitigate it?
A: Copying more data into a fixed-size stack buffer than it can hold (e.g.
   an unchecked strcpy) overwrites whatever the compiler placed next on
   the stack - historically the saved return address, letting an attacker
   redirect execution once the function returns. A stack canary
   (-fstack-protector[-strong]) places a random value right after local
   buffers, checked before the function returns; an overflow that reaches
   it triggers "*** stack smashing detected ***" and aborts instead of
   letting a corrupted return address run. The canary detects the
   corruption, it doesn't prevent the overflow itself. See
   `code/77_SecurityDemos/01_bufferOverflowStackCanary.c` - note this
   machine's default gcc doesn't enable the canary automatically, so the
   file's comment shows the flag needed to see the intended abort instead
   of a raw segfault.

Q: What is a format string vulnerability?
A: Passing attacker-controlled data directly as printf's *format string*
   (`printf(userInput)` instead of `printf("%s", userInput)`) lets that
   data's own `%x`/`%s`/`%n` directives control what printf reads (or, for
   `%n`, writes) - printf walks the varargs it expects for each directive
   regardless of whether the caller actually passed a matching argument,
   which is how it ends up reading (and printing) arbitrary stack/register
   contents. See `code/77_SecurityDemos/02_formatStringVulnerability.c`,
   which shows both the vulnerable call and the one-argument fix.

Q: How can integer overflow lead to a buffer overflow?
A: A size calculation like `elementCount * elementSize` can silently wrap
   around to a small number if the true product exceeds the integer
   type's range - code that then mallocs based on that wrapped (tiny)
   result, but later writes based on the original (huge) intended count,
   walks straight off the end of the undersized buffer it actually got.
   The fix is checking for overflow before multiplying, not trusting the
   result. See `code/77_SecurityDemos/03_integerOverflowToBufferOverflow.c`.

===========================================================================
Debugging/profiling and dynamic linking
===========================================================================

Q: How would you find a data race that a mutex fix in review might have
   missed?
A: ThreadSanitizer (-fsanitize=thread) instruments every memory access
   and reports the exact two conflicting accesses (source lines, threads)
   the instant it observes an unsynchronized read/write race - far more
   reliable than waiting for a visibly wrong result, since races don't
   necessarily lose data on every single run. See
   `code/78_ThreadSanitizerDemo/01_racyCounter.c` (deliberately
   unsynchronized on purpose, for this exact demo) - compile it with
   `-fsanitize=thread -pthread` to see the report.

Q: How does dlopen/dlsym enable a plugin architecture?
A: dlopen() loads a shared library at runtime by path, instead of it
   being resolved once at program startup - the set of loadable modules
   doesn't need to be known when the loading program was compiled.
   dlsym() then looks up a symbol by name string at runtime; the caller
   supplies the function pointer's type via a cast, since no type
   information is carried in the library itself - a mismatched cast
   compiles fine but misbehaves at the call. See
   `code/74_DynamicLoading/01_loader.c` and `plugin.c`.

Q: What does symbol visibility control, and why hide symbols by default?
A: -fvisibility=hidden makes every symbol in a shared library hidden
   (unreachable from outside) unless explicitly marked
   __attribute__((visibility("default"))). Real libraries do this to
   shrink the exported symbol table (faster dynamic linking), and to
   prevent callers from accidentally depending on internal-only
   functions that were never meant to be part of the public API. See
   `code/74_DynamicLoading/plugin.c`, where `pluginGreet` is exported and
   reachable via dlsym, but `pluginInternalHelper` (no visibility
   attribute) is not, even though it's ordinary, non-static code.

Q: Why does C++ code need extern "C" to call a C library?
A: A C++ compiler mangles function names to encode argument types
   (enabling overloading) - a name like `addTwo` looks nothing like what
   a plain C compiler emitted for the same function. extern "C" tells the
   C++ compiler to use plain C linkage for those declarations instead, so
   the linker can actually find the C-compiled symbols. See
   `code/75_ExternC/mathutils.h` (the extern "C" declarations, guarded by
   `#ifdef __cplusplus` so plain C includers see no change at all) and
   `01_main.cpp` (the C++ caller).

===========================================================================
Resource limits and real-time
===========================================================================

Q: What are ulimit/getrlimit/setrlimit for, and how does a fork bomb relate?
A: Every process has a soft limit (the current enforced cap, adjustable up
   to the hard limit) and a hard limit (the ceiling, raisable further only
   by root) per resource - RLIMIT_NOFILE (open file descriptors),
   RLIMIT_NPROC (process count), and others. A service that leaks file
   descriptors eventually fails with EMFILE once it hits RLIMIT_NOFILE;
   a fork bomb (a process endlessly fork()ing itself) is the same
   resource-exhaustion category applied to RLIMIT_NPROC, capping how many
   processes one user can ever have running - typically configured
   system-wide via /etc/security/limits.conf, not something to actually
   run as a demo. See `code/76_ResourceLimits/01_rlimitFileDescriptors.c`,
   which lowers its own RLIMIT_NOFILE and deliberately hits EMFILE.

Q: What can't you do inside an interrupt service routine (ISR), and how
   does that compare to a signal handler?
A: An ISR runs at a point where the OS can't safely do most of what
   ordinary code takes for granted: no blocking calls, no calling
   anything that might sleep or allocate memory (malloc can block/take a
   lock), and strict limits on how long it can run before it starts
   delaying other interrupts. This is a stricter version of the same
   constraint signal handlers have (see `47_SignalHandling/01_customHandler.c`
   and its `volatile sig_atomic_t`-only rule) - an ISR's environment is
   even more restricted than a signal handler's, since it may run with
   interrupts disabled and without a normal process context at all.

===========================================================================
Design patterns in plain C
===========================================================================

Q: How do you hide a struct's implementation in C (PIMPL-equivalent)?
A: Expose only a typedef'd pointer to an incomplete struct type in the
   header (`typedef struct Handle Handle;`, with no members declared) plus
   accessor functions; the actual struct definition lives only in the
   corresponding .c file. Callers can never access fields directly (there
   are none visible to them), can't be broken by a field reordering in a
   later version (the ABI-compatibility problem raised earlier in this
   file), and are forced through accessors that can enforce invariants a
   raw public field never could. See `code/36_OpaquePointer/handle.h`
   and `handle.c`.

Q: How would you emulate virtual dispatch/polymorphism in plain C?
A: A struct of function pointers (a hand-rolled vtable) plus a convention
   that every "subclass" embeds a pointer to its own vtable as its first
   member - close to literally what a C++ compiler generates for a class
   with virtual functions, just written out explicitly. Calling
   `shape->vtable->area(shape)` looks up and calls the right concrete
   function through that pointer, without the caller needing to know
   which concrete type it actually has. See
   `code/37_VTableEmulation/01_shapeVtable.c`.

===========================================================================
Process hygiene
===========================================================================

Q: Why do teams build with -Werror in CI?
A: It turns every compiler warning into a hard build failure instead of
   something easy to ignore in a wall of build output - most real bugs
   that a compiler warning catches (an uninitialized variable, a format
   string mismatch, a signed/unsigned comparison) are cheap to fix at the
   moment they're introduced and expensive to debug once they're a
   production incident. The tradeoff is that a new compiler version
   adding new warnings can suddenly break a build that was previously
   clean, which is why -Werror is often paired with pinning a specific
   compiler version in CI.

Q: How do you unit test C, given the language has no built-in test
   framework?
A: Real frameworks (CUnit, Check, Unity) add test discovery,
   setup/teardown, and structured reporting on top of the same core idea
   demonstrated in `code/38_UnitTesting/01_assertBasedTests.c`: call a
   function, compare its result to an expected value, record pass/fail,
   and keep running the rest of the suite instead of aborting on the
   first failure like a raw assert() would (see
   `18_ControlFlowExtras/02_assert.c` for the invariant-checking use case
   assert is actually meant for). Returning nonzero from the test binary
   when any test fails is what lets CI treat a failing suite as a failing
   build step, the same as -Werror does for warnings.

===========================================================================
Behavioral/architecture questions specific to seniority
===========================================================================

Q: Describe a time you rejected or heavily revised a junior's PR.
A: (Answer in this shape: what the PR did on the surface; the deeper
   issue you caught - often a subtle correctness bug, a missed edge case,
   or a design choice that would cause maintenance pain later; how you
   communicated it - specific, kind, and focused on the code/tradeoff
   rather than the person; and the outcome.)

Q: Describe a legacy codebase you refactored without breaking existing
   behavior.
A: (Answer in this shape: what made the code risky to touch - e.g. no
   tests, implicit global state, or unclear ownership of a data
   structure; what gave you confidence the refactor was safe - usually
   adding characterization tests *before* changing anything, or a
   staged rollout with monitoring; and the actual change.)

Q: Describe a tradeoff you made under a hard constraint (memory budget,
   real-time deadline, backward compatibility).
A: (Answer in this shape: the constraint, the options you considered, why
   you picked the one you did, and what you gave up to get it - a good
   answer names the cost explicitly, not just the benefit.)

===========================================================================
Codeable follow-ups
===========================================================================

All of the following are now added: a lock-free SPSC ring buffer
(`code/61_LockFreeRingBuffer/`), a false-sharing benchmark
(`code/62_FalseSharing/`), an arena/pool custom allocator
(`code/65_CustomAllocator/`), a strict-aliasing UB demonstration
(`code/42_StrictAliasing/`), select/poll/epoll (`code/71_IOMultiplexing/`),
UDP sockets (`code/70_UDPSockets/`), three security demos
(`code/77_SecurityDemos/`), a ThreadSanitizer-ready race
(`code/78_ThreadSanitizerDemo/`), dlopen/dlsym with symbol visibility
(`code/74_DynamicLoading/`), extern "C" C/C++ interop
(`code/75_ExternC/`), rlimit/EMFILE (`code/76_ResourceLimits/`), an opaque
pointer (`code/36_OpaquePointer/`), a hand-rolled vtable
(`code/37_VTableEmulation/`), and an assert-based mini test framework
(`code/38_UnitTesting/`).

Still not codeable, or deliberately not run as a live demo:
strace/ltrace/perf (external tools, not example programs to write - run
them against any existing binary in this repo instead, e.g.
`strace ./45_Processes/01_forkBasics`); RTOS/ISR constraints (no RTOS
environment available here); an actual fork bomb (would exhaust this
machine's process table for every user on it, not just this program -
`code/76_ResourceLimits/01_rlimitFileDescriptors.c`'s comment explains the
mitigation instead of running the problem).
