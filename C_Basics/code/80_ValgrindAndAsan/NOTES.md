# Valgrind and AddressSanitizer (ASan)

Sanitizer family relationship: `78_ThreadSanitizerDemo` covers ThreadSanitizer
(`-fsanitize=thread`), which detects *data races* between threads. This
folder covers the memory-safety sanitizers - Valgrind's Memcheck and
`-fsanitize=address` (ASan) - which detect leaks, use-after-free, and
out-of-bounds accesses instead. They are complementary, not overlapping:
a program can be race-free but still leak or overflow, or vice versa, and
you would reach for a different tool (or combination) for each.

Demo files, one bug each:
- `01_memoryLeak.c` - `malloc()` with no matching `free()`.
- `02_useAfterFree.c` - reads a heap buffer after it has been `free()`d.
- `03_bufferOverflow.c` - writes past the end of a heap allocation.

## Valgrind (Memcheck)

- Run without recompiling: `valgrind --leak-check=full ./01_memoryLeak`.
  Works on an ordinary optimized or unoptimized binary (debug symbols with
  `-g` make the report point at source lines instead of just addresses).
- Leak output for `01_memoryLeak.c` looks like:
  ```
  ==PID== HEAP SUMMARY:
  ==PID==     definitely lost: 64 bytes in 1 blocks
  ==PID== 1 bytes ... are definitely lost in loss record 1 of 1
  ==PID==    at 0x...: malloc (...)
  ==PID==    by 0x...: allocateBuffer (01_memoryLeak.c:14)
  ==PID==    by 0x...: main (01_memoryLeak.c:26)
  ```
  "definitely lost" means no pointer to that block exists anywhere at exit -
  an unambiguous leak (as opposed to "still reachable", which just means it
  was never freed but a pointer to it still exists, e.g. a global cache).
- For `02_useAfterFree.c`, Memcheck reports an "Invalid read" at the
  `buffer[0]` line, plus a second stack trace showing where the block was
  freed - the same "what happened, and where" pairing ASan gives.
- Runs the program under a full software emulator (no hardware
  instrumentation), so it is CPU-heavy (commonly 10-50x slowdown) but needs
  no special compiler flags or recompilation - useful for a binary you can't
  rebuild, or for third-party libraries.

## AddressSanitizer (ASan)

- Requires recompiling with instrumentation: `gcc -fsanitize=address -g
  01_memoryLeak.c -o leak_asan && ./leak_asan`.
- Runs at roughly 2x native speed (vs. Valgrind's order-of-magnitude
  slowdown) because the compiler inserts inline shadow-memory checks at
  build time instead of emulating every instruction.
- `03_bufferOverflow.c` under ASan reports a precise "heap-buffer-overflow"
  with the exact write address, size, and a stack trace to the `memset`
  call - Valgrind's *default* mode does not reliably catch small heap
  overflows that stay within the same malloc "red zone" slop the allocator
  happens to leave; ASan's redzones around every allocation are what make it
  reliably better at exactly this bug class.
- Leak detection is built into ASan by default (`LeakSanitizer`, runs at
  exit) and reports the same "leaked" summary style as Memcheck.
- `02_useAfterFree.c` under ASan reports "heap-use-after-free" with three
  stack traces: the bad read, the `free()` call, and the original
  `malloc()` - more context in one report than Memcheck's two-trace version.

## When to reach for which

- Valgrind: no recompilation needed, works on any binary (even without
  source), broader tool suite (Memcheck, Helgrind, Cachegrind, Massif) -
  but slow, and can miss some small/precisely-timed heap overflows in its
  default configuration.
- ASan: needs a rebuild with `-fsanitize=address`, but is much faster (so
  it's suitable for CI test suites, not just one-off debugging) and catches
  stack-buffer and small heap-buffer overflows more reliably via redzones.
- Common practice: run ASan continuously in CI/test builds; reach for
  Valgrind when you need to check something ASan can't (a shipped binary,
  or `--track-origins=yes`-style uninitialized-read root-causing) or want
  its other tools (e.g. Massif for heap profiling).

## Build note

- These three files compile cleanly under plain `-Wall` (the Makefile's
  default build). Some newer GCC versions may warn about the deliberate
  overflow in `03_bufferOverflow.c` (`-Wstringop-overflow`) since the
  destination size is a compile-time-visible constant - that warning is
  expected and correct here; it is the compiler noticing the exact bug the
  file exists to demonstrate.
