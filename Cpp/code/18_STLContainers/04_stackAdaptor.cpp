#include <iostream>
#include <stack>
#include <vector>
#include <list>

// std::stack is a container ADAPTOR, not a standalone container: it holds an
// underlying container (std::deque by default) and exposes only a
// push/pop/top LIFO interface on top of it. It can just as well be built on
// std::vector or std::list -- see the second example below.

using namespace std;

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates std::stack push/pop/top over its default deque
 *         backing, and rebinding the adaptor to vector/list backings.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    stack<int> deckOfCards;
    deckOfCards.push(1);
    deckOfCards.push(2);
    deckOfCards.push(3);

    cout << "stack (default deque-backed) pop order: ";
    while (!deckOfCards.empty())
    {
        cout << deckOfCards.top() << " ";
        deckOfCards.pop();
    }
    cout << "\n";

    // Same adaptor interface, explicitly backed by vector instead of deque.
    stack<int, vector<int>> vectorBackedStack;
    vectorBackedStack.push(10);
    vectorBackedStack.push(20);
    cout << "vector-backed stack top: " << vectorBackedStack.top() << "\n";

    // Same adaptor interface, backed by list.
    stack<int, list<int>> listBackedStack;
    listBackedStack.push(30);
    listBackedStack.push(40);
    cout << "list-backed stack top: " << listBackedStack.top() << "\n";

    return 0;
}
