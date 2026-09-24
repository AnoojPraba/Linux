#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
#include <atomic>

#define POOL_SIZE 4
#define TASK_COUNT 8

using namespace std;

// A minimal thread pool: a fixed set of worker threads pull tasks off a
// shared queue guarded by a mutex/condition_variable pair until told to stop.
class ThreadPool
{
    public:
        // trivial constructor, spawns the worker threads.
        explicit ThreadPool(int workerCount)
        {
            stopping = false;
            for (int i = 0; i < workerCount; i = i + 1)
            {
                workers.emplace_back(&ThreadPool::workerLoop, this);
            }
        }

        /*****************************************************************************
         * Name: submit
         *
         * Description:
         *         Adds a task to the queue and wakes one waiting worker.
         *
         * Inputs:
         *         task : the callable to run on a worker thread.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void submit(function<void()> task)
        {
            {
                lock_guard<mutex> guard(queueMutex);
                tasks.push(std::move(task));
            }
            taskAvailable.notify_one();
        }

        /*****************************************************************************
         * Name: ~ThreadPool
         *
         * Description:
         *         Signals all workers to stop once the queue drains, wakes
         *         them, and joins every worker thread.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        ~ThreadPool()
        {
            {
                lock_guard<mutex> guard(queueMutex);
                stopping = true;
            }
            taskAvailable.notify_all();
            for (thread &worker : workers)
            {
                worker.join();
            }
        }

    private:
        /*****************************************************************************
         * Name: workerLoop
         *
         * Description:
         *         Repeatedly waits for and runs queued tasks until stopping
         *         is set and the queue is empty.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void workerLoop()
        {
            while (true)
            {
                function<void()> task;
                {
                    unique_lock<mutex> lock(queueMutex);
                    taskAvailable.wait(lock, [this] { return (!tasks.empty()) || stopping; });

                    if (tasks.empty() && stopping)
                    {
                        return;
                    }

                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        }

        vector<thread> workers;
        queue<function<void()>> tasks;
        mutex queueMutex;
        condition_variable taskAvailable;
        bool stopping;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Submits a batch of tasks to a small thread pool and lets the
 *         pool's destructor drain and join all workers.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    atomic<int> completedCount(0);
    {
        ThreadPool pool(POOL_SIZE);
        for (int i = 0; i < TASK_COUNT; i = i + 1)
        {
            pool.submit([i, &completedCount]
            {
                cout << "running task " << i << " on thread "
                          << this_thread::get_id() << "\n";
                completedCount.fetch_add(1, memory_order_relaxed);
            });
        }
    }
    cout << "completed " << completedCount.load() << " tasks\n";
    return 0;
}
