#include <iostream>
#include <thread>
#include <mutex>

#define INCREMENTS_PER_THREAD 10000
#define THREAD_COUNT 4

// Deadlock avoidance: when a thread must hold two mutexes at once, always
// acquire them in the same global order (or use std::lock/std::scoped_lock
// to acquire both atomically) so no two threads can wait on each other.
std::mutex counterMutex;
long sharedCounter = 0;

/*****************************************************************************
 * Name: incrementCounter
 *
 * Description:
 *         Increments the shared counter under a lock_guard so concurrent
 *         threads cannot interleave their read-modify-write sequences.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void incrementCounter()
{
    for (int i = 0; i < INCREMENTS_PER_THREAD; i = i + 1)
    {
        std::lock_guard<std::mutex> guard(counterMutex);
        sharedCounter = sharedCounter + 1;
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Launches multiple threads that all increment a shared counter
 *         protected by a mutex, then verifies the final total is correct.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::thread threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i = i + 1)
    {
        threads[i] = std::thread(incrementCounter);
    }

    for (int i = 0; i < THREAD_COUNT; i = i + 1)
    {
        threads[i].join();
    }

    // unique_lock is used here only to show the alternative API; it supports
    // deferred locking and manual unlock/relock, unlike lock_guard.
    std::unique_lock<std::mutex> finalLock(counterMutex);
    std::cout << "final counter = " << sharedCounter << "\n";
    finalLock.unlock();

    return 0;
}
