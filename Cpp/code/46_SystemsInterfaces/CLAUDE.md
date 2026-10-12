# 46_SystemsInterfaces

Memory layout, name mangling and the ODR, errno/exceptions/signals, chrono, RAII sockets, command-line args with binary file I/O, and thread data races.

## Files
- `01_memoryLayoutCpp.cpp` - Where globals, static members, function-local statics, thread_local, heap, vector elements, locals and polymorphic objects live.
- `02_nameManglingAndOdr.cpp` - Mangled vs demangled type names, extern "C" linkage and overloads.
- `03_errnoExceptionsSignals.cpp` - errno -> std::system_error, std::errc comparison, sigaction with a lock-free atomic flag.
- `04_chronoClocks.cpp` - steady vs system clock, duration literals and casts, sleep_for/sleep_until.
- `05_socketRaii.cpp` - Move-only RAII file-descriptor wrapper; TCP server and client (client in a thread) over loopback.
- `06_argsAndBinaryRecords.cpp` - argv via span/string_view, binary records with seekg/seekp random access, trivially-copyable static_assert.
- `07_threadsMutexAtomic.cpp` - Four threads incrementing a counter: lock_guard vs std::atomic.
- `NOTES.md` - concise interview-focused notes for this topic

## Build and run
- Single file: `g++ -std=c++20 -Wall -Wextra -pthread 01_memoryLayoutCpp.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/46_SystemsInterfaces/` (git-ignored).

## Key concepts / interview angles
- **Layout:** text (code, vtables, rodata), data/bss (globals, `static` members, function-local statics - local statics initialize on first use, thread-safely), TLS block for `thread_local`, heap (`new`, `make_unique`, vector/string buffers), stack (automatic objects). A polymorphic object has a hidden vptr into read-only data. Static-initialization order across files is unspecified (use construct-on-first-use).
- **Mangling:** the compiler encodes namespace/class/parameter types into symbol names (`_Z3addii`); the scheme is ABI-specific (Itanium vs MSVC). `extern "C"` = C linkage, unmangled, no overloading; shared C/C++ headers wrap in `#ifdef __cplusplus`.
- **ODR:** one definition per function/object in the program; class/template/inline definitions must be identical in every TU. Headers hold declarations, templates, `inline`/`constexpr` functions; anonymous namespace gives per-file copies. Violations: multiple-definition link errors or silent UB.
- **Errors:** return value + `errno` (read it immediately, thread-local) -> `std::system_error(errno, std::generic_category())`; compare with `std::errc`. **Signals** are asynchronous: handler may only touch `volatile std::sig_atomic_t` or lock-free atomics; no iostream/new/locks/exceptions. Use `sigaction` not `signal`. SIGSEGV/SIGFPE are not exceptions.
- **chrono:** `steady_clock` for elapsed time/timeouts (monotonic), `system_clock` for calendar time (can jump), `high_resolution_clock` is just an alias. Typed durations: lossless conversions implicit, lossy need `duration_cast`.

## Gotchas
- `etext/edata/end` and `abi::__cxa_demangle` are GNU/Itanium specific.
- The socket demo needs -pthread (the Makefile already passes it).
