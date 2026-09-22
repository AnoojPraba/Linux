#include <iostream>
#include <thread>
#include <atomic>

#define INCREMENTS_PER_THREAD 100000
#define THREAD_COUNT 4

// C++'s std::atomic<T> is a typed template (std::atomic<int>, std::atomic<bool>, ...)
// with member functions like load()/store()/fetch_add(), whereas C's <stdatomic.h>
// exposes the same underlying compiler intrinsics through _Atomic-qualified types and
// free functions (atomic_load, atomic_fetch_add, ...). Both support the same
// std::memory_order values (relaxed, acquire, release, acq_rel, seq_cst).
std::atomic<long> atomicCounter(0);

/*****************************************************************************
 * Name: incrementAtomic
 *
 * Description:
 *         Increments the atomic counter without any external locking,
 *         relying on the default sequentially-consistent memory order.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void incrementAtomic()
{
    for (int i = 0; i < INCREMENTS_PER_THREAD; i = i + 1)
    {
        // memory_order_relaxed is enough here because we only care about the
        // final total, not about ordering this increment relative to other
        // memory operations on other variables.
        atomicCounter.fetch_add(1, std::memory_order_relaxed);
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Launches multiple threads incrementing a std::atomic counter and
 *         prints the final value once all threads complete.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::thread threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i = i + 1)
    {
        threads[i] = std::thread(incrementAtomic);
    }

    for (int i = 0; i < THREAD_COUNT; i = i + 1)
    {
        threads[i].join();
    }

    std::cout << "final atomic counter = " << atomicCounter.load() << "\n";
    return 0;
}
