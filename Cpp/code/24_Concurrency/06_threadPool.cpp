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
        void submit(std::function<void()> task)
        {
            {
                std::lock_guard<std::mutex> guard(queueMutex);
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
                std::lock_guard<std::mutex> guard(queueMutex);
                stopping = true;
            }
            taskAvailable.notify_all();
            for (std::thread &worker : workers)
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
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
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

        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable taskAvailable;
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
    std::atomic<int> completedCount(0);
    {
        ThreadPool pool(POOL_SIZE);
        for (int i = 0; i < TASK_COUNT; i = i + 1)
        {
            pool.submit([i, &completedCount]
            {
                std::cout << "running task " << i << " on thread "
                          << std::this_thread::get_id() << "\n";
                completedCount.fetch_add(1, std::memory_order_relaxed);
            });
        }
    }
    std::cout << "completed " << completedCount.load() << " tasks\n";
    return 0;
}
