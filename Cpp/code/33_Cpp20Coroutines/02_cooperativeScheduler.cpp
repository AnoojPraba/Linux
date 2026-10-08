#include <coroutine>
#include <deque>
#include <iostream>
#include <string>

// Cooperative multitasking with coroutines: a Task type plus an awaitable
// `yield_now()` that re-queues the coroutine on a round-robin scheduler. No
// threads, no locks - the "scheduler" is just a deque of handles. This is how
// async I/O frameworks (asio, libuv wrappers, seastar) structure code: write
// straight-line code, suspend at I/O waits, resume from the event loop.
struct Scheduler
{
    std::deque<std::coroutine_handle<>> ready;
    void spawn(std::coroutine_handle<> h) { ready.push_back(h); }
    void run()
    {
        while (!ready.empty())
        {
            auto h = ready.front();
            ready.pop_front();
            h.resume();                       // runs until next suspension or end
        }
    }
};

static Scheduler g_sched;

struct YieldNow                                // an awaiter
{
    bool await_ready() const noexcept { return false; }          // always suspend
    void await_suspend(std::coroutine_handle<> h) const { g_sched.spawn(h); }
    void await_resume() const noexcept {}
};
static YieldNow yield_now() { return {}; }

struct Task
{
    struct promise_type
    {
        Task get_return_object()
        {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        // Final suspend never suspends: the frame destroys itself on completion
        // (fire-and-forget task; handle in Task is then NOT owned).
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> handle;
};

// NOTE the by-value parameter: coroutine frames outlive the caller's stack, so
// taking `const std::string &name` of a temporary would dangle.
Task worker(std::string name, int steps)
{
    for (int i = 1; i <= steps; ++i)
    {
        std::cout << name << " step " << i << "/" << steps << '\n';
        co_await yield_now();                   // give other tasks a turn
    }
    std::cout << name << " done\n";
}

int main()
{
    g_sched.spawn(worker("A", 3).handle);
    g_sched.spawn(worker("B", 2).handle);
    g_sched.spawn(worker("C", 4).handle);
    g_sched.run();                              // interleaved: A1 B1 C1 A2 B2 C2 ...
}
