#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#define ITEM_COUNT 5

std::mutex queueMutex;
std::condition_variable queueCondition;
std::queue<int> sharedQueue;
bool productionDone = false;

/*****************************************************************************
 * Name: producer
 *
 * Description:
 *         Pushes a fixed number of items onto the shared queue, notifying
 *         the consumer after each push, then signals completion.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void producer()
{
    for (int i = 0; i < ITEM_COUNT; i = i + 1)
    {
        {
            std::lock_guard<std::mutex> guard(queueMutex);
            sharedQueue.push(i);
            std::cout << "produced " << i << "\n";
        }
        queueCondition.notify_one();
    }

    {
        std::lock_guard<std::mutex> guard(queueMutex);
        productionDone = true;
    }
    queueCondition.notify_one();
}

/*****************************************************************************
 * Name: consumer
 *
 * Description:
 *         Waits on the condition variable until items are available or
 *         production has finished, draining the queue as items arrive.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void consumer()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCondition.wait(lock, [] { return (!sharedQueue.empty()) || productionDone; });

        while (!sharedQueue.empty())
        {
            int value = sharedQueue.front();
            sharedQueue.pop();
            std::cout << "consumed " << value << "\n";
        }

        if (productionDone)
        {
            break;
        }
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Runs a producer and consumer thread coordinated through a
 *         condition variable, demonstrating the classic bounded-work
 *         hand-off pattern without busy-waiting.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}
