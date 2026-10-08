# C++20 Coroutines

Build: the Makefile compiles this folder with `-std=c++20` (GCC >= 10).

## Model
- A coroutine is any function containing `co_await`, `co_yield` or `co_return`.
  It is **stackless**: locals live in a compiler-generated **coroutine frame**
  (usually heap-allocated, may be elided by HALO optimization), not on the
  caller's stack. Suspension returns to the caller; the frame persists until
  `destroy()`.
- Three cooperating pieces:
  - **promise_type:** customization point - `get_return_object`,
    `initial_suspend`, `final_suspend`, `yield_value`, `return_value/void`,
    `unhandled_exception`. One promise per coroutine invocation.
  - **coroutine_handle<>:** non-owning pointer to the frame: `resume()`,
    `done()`, `destroy()`, `promise()`.
  - **awaitable/awaiter:** what `co_await x` calls: `await_ready()` (skip
    suspension?), `await_suspend(h)` (runs after suspending - schedule the
    resume, e.g. on an I/O completion), `await_resume()` (the result).
- `co_yield v` == `co_await promise.yield_value(v)`. `co_return v` ->
  `promise.return_value(v)`.
- Flow: allocate frame -> construct promise -> `get_return_object()` -> `co_await
  initial_suspend()` -> body -> `co_await final_suspend()`.
- `initial_suspend` = `suspend_always` -> **lazy** (starts on first resume);
  `suspend_never` -> **eager**. `final_suspend` should be `noexcept`; suspending
  there keeps the frame so the owner can read the result and destroy it.

## Demos
- `01_generator.cpp`: lazy `Generator<T>` (infinite Fibonacci, range) - the owner
  destroys the frame in its destructor (RAII).
- `02_cooperativeScheduler.cpp`: round-robin scheduler of detached tasks using an
  awaitable that re-queues itself; output interleaves A/B/C without threads.

## Pitfalls (high-value interview material)
- **Dangling references:** parameters taken by reference refer to the caller's
  objects, which may be gone after the first suspension. Take parameters BY VALUE.
  Same for lambda coroutines - the lambda's captures live in the closure object,
  not in the coroutine frame; a temporary closure that dies before the coroutine
  finishes = use-after-free.
- **Lifetime/ownership:** forgetting `destroy()` leaks the frame; destroying
  twice or after self-destroy at `final_suspend` (suspend_never) is UB.
- **Exceptions:** thrown inside the body go to `unhandled_exception()`; decide to
  store (`exception_ptr`), rethrow, or terminate.
- **Heap allocation per call** (unless elided): can matter in hot paths; customize
  `operator new` in the promise or use pmr allocators.
- **Symmetric transfer:** returning a `coroutine_handle` from `await_suspend`
  tail-resumes it without growing the stack (needed to avoid stack overflow in
  long chains of awaits).
- **Threads:** resuming on a different thread than the one that suspended is the
  awaiter's decision; races on shared state remain your problem.
- GCC 10-11 need `-fcoroutines` with `-std=c++20`; GCC 12 enables it
  under `-std=c++20`.

## Stackless vs stackful
| | Stackless (C++20, Rust async, Python) | Stackful (fibers, Go goroutines, `ucontext`) |
|---|---|---|
| Suspend from | only the coroutine function itself | any depth of nested calls |
| Memory | one small frame per coroutine | a whole stack per coroutine (KBs-MBs) |
| Cost | compile-time state machine | context switch (register save/restore) |
| Viral annotation | yes (callers must be coroutines to await) | no |
C analogues: `../../../C_Basics/code/82_CoroutinesInC` (ucontext stackful and
`switch`-based stackless).

## Where coroutines are used
Async I/O (asio `awaitable`, io_uring wrappers), generators/ranges pipelines
(`std::generator` in C++23), lazy computation, game scripting, state machines,
parsers. Compare with callbacks (inversion of control, "callback hell"),
futures/promises (heap + allocation per continuation) and threads (OS cost).

## Senior interviewer Q&A
**Q: How does a C++20 coroutine differ from a thread?**
A: A thread is scheduled preemptively by the OS and has its own stack;
a coroutine is cooperative, runs on whichever thread resumes it, and only
yields at explicit suspension points. Thousands of coroutines cost KBs each, not
MBs, and switching is a function call, not a kernel context switch.

**Q: What does the compiler generate for a coroutine?**
A: A state machine: a frame struct holding parameters (copied), the promise,
live locals across suspension points and a resume-index; plus `resume`/`destroy`
functions that switch on the index. The caller gets the object produced by
`promise.get_return_object()`.

**Q: What are the dangers of coroutines + references/lambdas?**
A: See pitfalls: the frame outlives the call expression; references and lambda
captures can dangle. Pass by value, keep closures alive (static function or
named closure), or use `co_await` on owned objects.

**Q: How would you implement `co_await sock.read()` on epoll/io_uring?**
A: `await_ready` returns false; `await_suspend(h)` registers the fd with the
event loop with a callback that calls `h.resume()` on readiness (or stores `h`
in the io_uring user_data and resumes from the completion handler);
`await_resume` returns the bytes read. See `../../../OS/code/67_EpollInDepth`.

**Q: What is `final_suspend` for?**
A: To decide whether the frame is destroyed automatically or kept alive after
completion so the owner can retrieve the result/exception and destroy it.
Suspending at the end also lets awaiting coroutines be resumed (continuation
chaining).

**Q: Why doesn't `std::generator` / coroutine support exist in C?**
A: C has no such language feature; people emulate with `ucontext`/hand-written
fibers, `setjmp/longjmp`, or macro-based state machines (Duff's device
protothreads) - see the C folder referenced above.
