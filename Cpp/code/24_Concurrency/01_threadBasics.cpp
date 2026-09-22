#include <iostream>
#include <thread>
#include <string>

#define WORKER_ITERATIONS 3

/*****************************************************************************
 * Name: printMessage
 *
 * Description:
 *         Prints a message a fixed number of times, used as a thread entry
 *         point to show a function running on its own std::thread.
 *
 * Inputs:
 *         label : text identifying which caller is printing.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void printMessage(const std::string &label)
{
    for (int i = 0; i < WORKER_ITERATIONS; i = i + 1)
    {
        std::cout << label << " iteration " << i << "\n";
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Creates two std::thread objects running printMessage with
 *         different arguments, then joins both before exiting.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::thread worker1(printMessage, "worker1");
    std::thread worker2(printMessage, "worker2");

    worker1.join();
    worker2.join();

    std::cout << "both threads finished\n";
    return 0;
}
