#include <iostream>
#include <list>

// std::list is a doubly-linked list -- the STL's idiomatic answer to a
// from-scratch linked list. Unlike std::vector, inserting/erasing in the
// middle given an iterator is O(1) (no shifting of subsequent elements),
// at the cost of no random access and worse cache locality.

using namespace std;

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates std::list push_front/push_back, mid-list insert/erase
 *         via an iterator, and splice moving elements between two lists.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    list<int> numbers;
    numbers.push_back(2);
    numbers.push_back(3);
    numbers.push_front(1);
    numbers.push_back(5);

    cout << "list after push_front/push_back: ";
    for (int value : numbers)
    {
        cout << value << " ";
    }
    cout << "\n";

    // Insert 4 before the element holding 5. Walking to the position still
    // costs O(n), but the insert/erase step itself is O(1) once you're there
    // -- a vector would additionally need to shift every following element.
    auto it = numbers.begin();
    while ((it != numbers.end()) && (*it != 5))
    {
        ++it;
    }
    numbers.insert(it, 4);

    cout << "list after mid-list insert: ";
    for (int value : numbers)
    {
        cout << value << " ";
    }
    cout << "\n";

    // Erase the element holding 3 -- O(1) given the iterator, unlike
    // vector::erase which is O(n) due to shifting.
    it = numbers.begin();
    while ((it != numbers.end()) && (*it != 3))
    {
        ++it;
    }
    if (it != numbers.end())
    {
        numbers.erase(it);
    }

    cout << "list after mid-list erase: ";
    for (int value : numbers)
    {
        cout << value << " ";
    }
    cout << "\n";

    // splice: move all elements of "extra" into "numbers" without copying
    // any of them -- it just relinks the underlying nodes.
    list<int> extra = {100, 200};
    numbers.splice(numbers.end(), extra);

    cout << "list after splice: ";
    for (int value : numbers)
    {
        cout << value << " ";
    }
    cout << "\n";
    cout << "extra is empty after splice: " << (extra.empty() ? "true" : "false") << "\n";

    return 0;
}
