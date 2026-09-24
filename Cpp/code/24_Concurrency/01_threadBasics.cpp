#include <iostream>
#include <thread>
#include <string>

#define WORKER_ITERATIONS 3

using namespace std;

/*****************************************************************************
 * Name: printMessage
 *
 * Description:
 *         Prints a message a fixed number of times, used as a thread entry
 *         point to show a function running on its own thread.
 *
 * Inputs:
 *         label : text identifying which caller is printing.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void printMessage(const string &label)
{
    for (int i = 0; i < WORKER_ITERATIONS; i = i + 1)
    {
        cout << label << " iteration " << i << "\n";
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Creates two thread objects running printMessage with
 *         different arguments, then joins both before exiting.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    thread worker1(printMessage, "worker1");
    thread worker2(printMessage, "worker2");

    worker1.join();
    worker2.join();

    cout << "both threads finished\n";
    return 0;
}
