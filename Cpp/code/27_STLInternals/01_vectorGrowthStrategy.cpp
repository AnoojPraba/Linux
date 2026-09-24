#include <iostream>
#include <vector>

#define PUSH_COUNT 20

using namespace std;

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Pushes values onto a vector one at a time, printing size() versus
 *         capacity() after each push to show the amortized-doubling growth
 *         strategy: capacity jumps only occasionally, each time roughly
 *         doubling, so the amortized cost per push_back stays O(1).
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    vector<int> values;
    size_t previousCapacity = values.capacity();

    for (int i = 0; i < PUSH_COUNT; i = i + 1)
    {
        values.push_back(i);
        if (values.capacity() != previousCapacity)
        {
            cout << "size = " << values.size() << ", capacity grew to "
                      << values.capacity() << "\n";
            previousCapacity = values.capacity();
        }
    }

    return 0;
}
