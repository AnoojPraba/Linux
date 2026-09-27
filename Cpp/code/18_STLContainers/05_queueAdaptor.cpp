#include <iostream>
#include <queue>
#include <list>

// std::queue is also a container ADAPTOR (like std::stack and
// std::priority_queue), backed by std::deque by default. It restricts the
// underlying container's interface down to a FIFO view: push at the back,
// pop from the front.

using namespace std;

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates std::queue push/pop/front/back over its default
 *         deque backing.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    queue<int> ticketLine;
    ticketLine.push(1);
    ticketLine.push(2);
    ticketLine.push(3);

    cout << "queue front: " << ticketLine.front() << ", back: " << ticketLine.back() << "\n";

    cout << "queue (default deque-backed) pop order: ";
    while (!ticketLine.empty())
    {
        cout << ticketLine.front() << " ";
        ticketLine.pop();
    }
    cout << "\n";

    // Same adaptor interface, backed by list instead of deque.
    queue<int, list<int>> listBackedQueue;
    listBackedQueue.push(10);
    listBackedQueue.push(20);
    cout << "list-backed queue front: " << listBackedQueue.front() << "\n";

    return 0;
}
