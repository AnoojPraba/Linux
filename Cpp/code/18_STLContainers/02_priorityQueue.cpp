#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <string>

#define TASK_LOW_PRIORITY 1
#define TASK_HIGH_PRIORITY 9

// std::priority_queue is a container ADAPTOR: it wraps an underlying container
// (std::vector by default) and restricts access to "top of the heap" only,
// maintaining the heap invariant internally via push_heap/pop_heap style logic.
// Conceptually this is the same array-based binary heap you'd build from
// scratch (see C_Basics' Heap folder for a from-scratch array-based
// implementation) -- the STL version just hides the sift-up/sift-down
// bookkeeping behind push()/pop()/top().

using namespace std;

struct Task
{
    string name;
    int priority;
};

/*****************************************************************************
 * Name: TaskLowerPriorityFirst (struct)
 *
 * Description:
 *         Custom comparator for a priority_queue<Task> so the task with the
 *         highest priority field is always at the top, mirroring the default
 *         max-heap semantics but for a user-defined type.
 *****************************************************************************/
struct TaskLowerPriorityFirst
{
    bool operator()(const Task &left, const Task &right) const
    {
        return left.priority < right.priority;
    }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates std::priority_queue's default max-heap behavior, a
 *         min-heap variant via std::greater<T>, and a custom comparator that
 *         orders a struct by one of its fields.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    // Default: max-heap, backed by vector<int>, compares with std::less<int>.
    priority_queue<int> maxHeap;
    maxHeap.push(5);
    maxHeap.push(1);
    maxHeap.push(9);
    maxHeap.push(3);

    cout << "max-heap pop order: ";
    while (!maxHeap.empty())
    {
        cout << maxHeap.top() << " ";
        maxHeap.pop();
    }
    cout << "\n";

    // Min-heap: swap the comparator to std::greater<T>, keep vector as the
    // underlying container.
    priority_queue<int, vector<int>, greater<int>> minHeap;
    minHeap.push(5);
    minHeap.push(1);
    minHeap.push(9);
    minHeap.push(3);

    cout << "min-heap pop order: ";
    while (!minHeap.empty())
    {
        cout << minHeap.top() << " ";
        minHeap.pop();
    }
    cout << "\n";

    // Custom comparator on a struct: orders Tasks by priority field, highest
    // first.
    priority_queue<Task, vector<Task>, TaskLowerPriorityFirst> taskQueue;
    taskQueue.push({"cleanup", TASK_LOW_PRIORITY});
    taskQueue.push({"fix outage", TASK_HIGH_PRIORITY});
    taskQueue.push({"write docs", TASK_LOW_PRIORITY + 1});

    cout << "task pop order:\n";
    while (!taskQueue.empty())
    {
        const Task &top = taskQueue.top();
        cout << "  " << top.name << " (priority " << top.priority << ")\n";
        taskQueue.pop();
    }

    return 0;
}
