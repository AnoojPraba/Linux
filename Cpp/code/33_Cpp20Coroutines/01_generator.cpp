#include <coroutine>
#include <cstdint>
#include <exception>
#include <iostream>
#include <utility>

// A C++20 coroutine is a function that can suspend (co_yield / co_await) and
// resume later. The compiler turns it into a heap-allocated "coroutine frame"
// (locals + resume point) plus a promise object that you customize via
// promise_type. This Generator is the canonical minimal example: a lazy
// sequence whose values are produced on demand.
template <typename T>
class Generator
{
public:
    struct promise_type
    {
        T current{};
        std::exception_ptr error;

        Generator get_return_object()
        {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }   // lazy start
        std::suspend_always final_suspend() noexcept { return {}; }     // keep frame for done()
        std::suspend_always yield_value(T v) noexcept
        {
            current = std::move(v);
            return {};                       // suspend, handing control back
        }
        void return_void() noexcept {}
        void unhandled_exception() { error = std::current_exception(); }
    };

    explicit Generator(std::coroutine_handle<promise_type> h) : h_(h) {}
    Generator(Generator &&o) noexcept : h_(std::exchange(o.h_, {})) {}
    Generator(const Generator &) = delete;
    Generator &operator=(Generator &&) = delete;
    ~Generator() { if (h_) h_.destroy(); }  // YOU own the frame: destroy it

    // Resume until the next co_yield; false when the coroutine finished.
    bool next()
    {
        if (!h_ || h_.done())
            return false;
        h_.resume();
        if (h_.promise().error)
            std::rethrow_exception(h_.promise().error);
        return !h_.done();
    }
    const T &value() const { return h_.promise().current; }

private:
    std::coroutine_handle<promise_type> h_;
};

Generator<std::uint64_t> fibonacci()
{
    std::uint64_t a = 0, b = 1;
    for (;;)                                 // infinite sequence, no memory growth
    {
        co_yield a;
        auto next = a + b;
        a = b;
        b = next;
    }
}

Generator<int> range(int first, int last)
{
    for (int i = first; i < last; ++i)
        co_yield i;
}

int main()
{
    auto fib = fibonacci();
    std::cout << "fib:";
    for (int i = 0; i < 12 && fib.next(); ++i)
        std::cout << ' ' << fib.value();
    std::cout << '\n';

    auto r = range(3, 8);
    std::cout << "range:";
    while (r.next())
        std::cout << ' ' << r.value();
    std::cout << '\n';
    // Both frames are destroyed by ~Generator; the infinite generator never
    // ran to completion, which is fine - destroy() on a suspended frame runs
    // the locals' destructors.
}
