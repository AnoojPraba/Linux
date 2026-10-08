# 33_Cpp20Coroutines

C++20 coroutines from first principles: a generator with co_yield and a cooperative round-robin scheduler, with a senior Q&A.

## Files
- `01_generator.cpp` - Generator type with promise_type; co_yield produces fibonacci and a range
- `02_cooperativeScheduler.cpp` - Task type and awaitable yield_now() re-queued on a round-robin scheduler (no threads or locks)
- `NOTES.md` - model, demos, pitfalls, stackless vs stackful, where used, "Senior interviewer Q&A"

## Build and run
- Needs C++20: `g++ -std=c++20 -Wall -Wextra -pthread 01_generator.cpp -o /tmp/x && /tmp/x` (g++ 12 here needs no extra `-fcoroutines`).
- `make` from `..` builds it with `-std=c++20` (the Makefile has a per-folder rule for `33_*` and `34_*`).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/33_Cpp20Coroutines/` (git-ignored).

## Key concepts / interview angles
- The compiler turns the coroutine into a heap-allocated frame plus a promise object; `co_await`, `co_yield`, `co_return` drive it via promise/awaiter hooks.
- Pitfalls: dangling reference parameters, leaking the frame without `destroy()`, exceptions go to `unhandled_exception()`, resuming on another thread.
- `final_suspend` and symmetric transfer (returning a handle from `await_suspend`).
- Stackless (C++20) vs stackful (fibers, ucontext); compare `../../../C_Basics/code/82_CoroutinesInC`.
- Integration with epoll/io_uring for `co_await socket.read()`.

## Related
- `../../../C_Basics/code/82_CoroutinesInC`
- `../../../OS/code/67_EpollInDepth`
- `../24_Concurrency`
- `../30_Cpp20Features`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
