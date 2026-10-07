# Signal Safety, Thread-Local Storage and Once-Init

- **Async-signal-safety:** a handler can interrupt the program anywhere,
  even inside `malloc`/`printf` holding a lock. Calling those from the handler
  can deadlock or corrupt state. Only functions on the POSIX async-signal-safe
  list (`man 7 signal-safety`): `write`, `_exit`, `kill`, `sigaction`,
  `read`, `open`/`close`, `signal`, `sem_post`, ... NOT `printf`, `malloc`,
  `free`, `syslog`, anything taking a stdio/heap lock.
- Shared state with a handler: `volatile sig_atomic_t` or a lock-free atomic.
  `volatile` alone is not atomic for wider types and gives no ordering.
- **Handler rules:** save/restore `errno`; keep it tiny; set a flag or write
  to a pipe and do real work in the main loop (**self-pipe trick**;
  modern alternatives `signalfd(2)`, `eventfd`, or `sigwait` in a dedicated
  thread with the signal blocked everywhere else).
- `sigaction` over `signal()`: defined semantics (handler stays installed,
  signal masked during the handler, `SA_RESTART` control) vs. `signal()`'s
  historically varying behavior.
- `SA_RESTART`: interrupted slow syscalls (`read`, `accept`) resume instead of
  failing with `EINTR`. Without it, loops must retry on `EINTR`.
- `SIGKILL`/`SIGSTOP` cannot be caught. Standard signals do not queue
  (multiple pending coalesce into one); real-time signals (`SIGRTMIN+`) queue
  and carry a value (`sigqueue`).
- In multithreaded programs a process-directed signal goes to an arbitrary
  thread that does not block it; use `pthread_sigmask` + a dedicated signal
  thread. `fork` in a multithreaded program: only the calling thread survives
  in the child, so only async-signal-safe calls are legal before `exec`.
- Synchronous faults (`SIGSEGV`, `SIGFPE`) retrigger forever if the handler
  just returns - exit or `siglongjmp`.
- See `../../../OS/code/06_SignalHandling` and `../../Notes/06_Error_Signals.c`.

## Thread-local storage
- `_Thread_local` (C11) / `__thread` (GNU): one instance per thread,
  initialized at thread start (static initializer only). Cost: extra
  indirection through the thread pointer (`fs`/`tpidr_el0`); dynamically
  loaded libraries need a slower general-dynamic TLS model.
- Uses: `errno`, per-thread caches/allocators (tcmalloc, jemalloc tcaches),
  avoiding lock contention on counters (then aggregate), reentrant legacy APIs
  (`strtok` -> `strtok_r`, `rand` -> `rand_r`).
- `pthread_key_create` gives dynamic TLS with destructors on thread exit.
- Pitfall: TLS values are per-thread, so a pointer to one is useless to other
  threads, and thread pools reuse threads (stale state leaks between tasks).

## One-time initialization
- `pthread_once` / C11 `call_once` guarantee a single run and that other
  callers wait for its completion - the safe replacement for hand-rolled
  double-checked locking, which is a data race without proper atomics
  (acquire/release; see `../../../OS/code/20_Atomics`).

## Senior interviewer Q&A
**Q: Why can't you call `printf` from a signal handler?**
A: The handler may interrupt `printf`/`malloc` mid-operation while they hold
internal locks or have inconsistent state; re-entering deadlocks or corrupts
the heap/stdio buffers. Only async-signal-safe functions (`man 7
signal-safety`) are permitted, e.g. `write`. Best practice: set a
`volatile sig_atomic_t` flag or write a byte to a pipe, and do the work in the
main loop.

**Q: Why `sigaction` instead of `signal`?**
A: `signal()` semantics vary (BSD vs SysV: handler reset, restart behavior,
masking). `sigaction` gives defined behavior: choose `SA_RESTART`,
`SA_SIGINFO`, mask additional signals during the handler, block until done.

**Q: What is `EINTR` and how do you handle it?**
A: A blocking syscall interrupted by a signal returns -1 with `errno ==
EINTR` unless `SA_RESTART` applies (and some calls, such as `epoll_wait`,
`select`, `nanosleep`, are never auto-restarted). Loop:
`while ((n = read(fd, buf, len)) < 0 && errno == EINTR);`.

**Q: How do you handle signals in a multithreaded program?**
A: Process-directed signals go to any thread that does not block them. Block
signals in all threads (`pthread_sigmask` before creating workers) and
dedicate one thread to `sigwait` / `signalfd`, converting signals into normal
events. Avoid handlers entirely if possible.

**Q: Do signals queue?**
A: Standard signals do not: several pending instances coalesce into one, so
you cannot count them. Real-time signals (`SIGRTMIN..SIGRTMAX`) queue in
order and carry data via `sigqueue`.

**Q: What happens if a `SIGSEGV` handler returns?**
A: The faulting instruction re-executes and faults again - infinite loop.
Handlers for synchronous faults must exit, or `siglongjmp` away; crash
reporters should dump state with `write` and `_exit`/re-raise with the
default action.

**Q: How does `_Thread_local` work, and what does it cost?**
A: Per-thread instance addressed relative to a thread pointer (`fs` on x86-64,
`tpidr_el0` on AArch64): static executables/initial-exec model is one load,
shared-library (general dynamic) TLS calls `__tls_get_addr`. Memory cost per
thread; initialization only with constant initializers (C), destructors need
`pthread_key_create` (or C++ `thread_local`).
*Follow-up: why is `errno` thread-local?* So concurrent syscalls in different
threads don't clobber each other's error codes; `errno` is a macro for
`*__errno_location()`.

**Q: Is double-checked locking safe in C?**
A: Not with plain variables - the unsynchronized first check is a data race
and may see a half-initialized object. Use `pthread_once`/`call_once`, or an
atomic flag with acquire/release (store-release after init, load-acquire on the
fast path).

**Q: `volatile sig_atomic_t` vs `atomic_int` for a handler flag?**
A: `sig_atomic_t` is the only integer type the standard guarantees safe for
handler/main communication (single-thread). Lock-free `atomic_int` also works
and additionally provides inter-thread ordering. Plain `int` or a `volatile
int` modified non-atomically is not guaranteed.
