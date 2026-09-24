#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

#define WORKER_COUNT 4

using namespace std;

// std::call_once + std::once_flag guarantee an initializer runs exactly once,
// even when many threads race to call it, without any hand-rolled
// double-checked-locking. It is the modern replacement for the classic
// "check flag, lock, check flag again" singleton pattern.
//
// Note: DesignPatterns/Creational/Singleton.cpp already gets this for free
// via a function-local static (Logger::instance()'s "static Logger single;")
// — C++11 guarantees magic-statics initialization is thread-safe, so that
// singleton needs no call_once. call_once is what you would reach for
// instead if the singleton could not use a function-local static (e.g. it
// needs extra one-time setup beyond constructing a single object).
once_flag initFlag;
int expensiveResource = 0;

/*****************************************************************************
 * Name: initializeResource
 *
 * Description:
 *         Performs the (simulated) expensive one-time initialization of the
 *         shared resource. Only ever runs once no matter how many threads
 *         call callOnceExample concurrently.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void initializeResource()
{
    cout << "initializing expensive resource on thread "
              << this_thread::get_id() << "\n";
    expensiveResource = 42;
}

/*****************************************************************************
 * Name: useResource
 *
 * Description:
 *         Ensures the shared resource is initialized exactly once via
 *         std::call_once, then reads it.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void useResource()
{
    call_once(initFlag, initializeResource);
    cout << "thread " << this_thread::get_id()
              << " sees resource = " << expensiveResource << "\n";
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Launches several threads that all race to initialize the same
 *         resource through std::call_once, demonstrating that the
 *         initializer runs exactly once.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    vector<thread> workers;

    for (int i = 0; i < WORKER_COUNT; i = i + 1)
    {
        workers.emplace_back(useResource);
    }

    for (thread &worker : workers)
    {
        worker.join();
    }

    return 0;
}
