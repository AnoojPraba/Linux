// Four threads incrementing a counter: lock_guard vs std::atomic.

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main()
{
    const int threads = 4, per_thread = 100000;

    long locked_total = 0;
    std::mutex m;
    std::atomic<long> atomic_total{0};

    std::vector<std::thread> pool;
    for (int t = 0; t < threads; ++t)
        pool.emplace_back([&] {
            for (int i = 0; i < per_thread; ++i)
            {
                {
                    std::lock_guard<std::mutex> lock(m);     // released automatically at the end of the block
                    ++locked_total;
                }
                ++atomic_total;                              // one indivisible read-modify-write
            }
        });
    for (auto &th : pool)
        th.join();                                           // wait for every thread before using the results

    std::cout << "with mutex : " << locked_total << '\n';
    std::cout << "with atomic: " << atomic_total << '\n';
    std::cout << "expected   : " << static_cast<long>(threads) * per_thread << '\n';
}
