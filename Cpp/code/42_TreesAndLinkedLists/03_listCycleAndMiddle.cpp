// Slow/fast pointers: middle of a linked list and cycle detection.

#include <iostream>
#include <memory>
#include <vector>

struct Node
{
    int val;
    Node *next = nullptr;
};

Node *middle(Node *head)
{
    Node *slow = head, *fast = head;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;                         // the second middle when the length is even
}

bool has_cycle(Node *head)
{
    Node *slow = head, *fast = head;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            return true;                 // the fast pointer lapped the slow one
    }
    return false;
}

int main()
{
    std::vector<std::unique_ptr<Node>> pool;       // owns the nodes, so nothing needs deleting by hand
    for (int v = 1; v <= 5; ++v)
        pool.push_back(std::make_unique<Node>(Node{v}));
    for (std::size_t i = 0; i + 1 < pool.size(); ++i)
        pool[i]->next = pool[i + 1].get();         // 1 -> 2 -> 3 -> 4 -> 5

    Node *head = pool.front().get();
    std::cout << "middle = " << middle(head)->val << '\n';
    std::cout << std::boolalpha << "cycle before: " << has_cycle(head) << '\n';

    pool.back()->next = pool[1].get();             // 5 now points back to 2
    std::cout << "cycle after : " << has_cycle(head) << '\n';
}
