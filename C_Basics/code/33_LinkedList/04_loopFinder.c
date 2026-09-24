#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int value;
    struct Node *next;
} Node;

Node *createNode(int value)
{
    Node *node = malloc(sizeof(Node));

    node->value = value;
    node->next = NULL;
    return node;
}

// Floyd's cycle detection (the "tortoise and hare"): slow moves one node per
// step, fast moves two. If there's a loop, fast will eventually lap slow and
// they meet inside the loop; if the list is acyclic, fast reaches NULL first.
int hasLoop(Node *head)
{
    Node *slow = head;
    Node *fast = head;

    while ((fast != NULL) && (fast->next != NULL))
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
        {
            return 1;
        }
    }
    return 0;
}

// Once a loop is detected at the slow/fast meeting point, resetting one
// pointer to head and advancing both one step at a time finds the loop's
// start node - both pointers are now the same distance from the start.
Node *findLoopStart(Node *head)
{
    Node *slow = head;
    Node *fast = head;

    while ((fast != NULL) && (fast->next != NULL))
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
        {
            slow = head;
            while (slow != fast)
            {
                slow = slow->next;
                fast = fast->next;
            }
            return slow;
        }
    }
    return NULL;
}

#define VISITED_TABLE_SIZE 128

typedef struct VisitedEntry
{
    Node *node;
    struct VisitedEntry *next;
} VisitedEntry;

// A pointer value works fine as a hash key - it's just the node's address.
// This is a simple bucket-chained hash set of the pointers seen so far, an
// O(n) time / O(n) space alternative to Floyd's O(n) time / O(1) space
// approach above: trading memory for a more direct "have I seen this node
// before" check instead of the two-pointer race.
unsigned int hashPointer(Node *p)
{
    return ((unsigned long)p / sizeof(Node)) % VISITED_TABLE_SIZE;
}

Node *findLoopStartHash(Node *head)
{
    VisitedEntry *table[VISITED_TABLE_SIZE] = {NULL};
    Node *current = head;
    Node *result = NULL;
    int i;

    while (current != NULL)
    {
        unsigned int bucket = hashPointer(current);
        VisitedEntry *entry = table[bucket];
        int seen = 0;

        while (entry != NULL)
        {
            if (entry->node == current)
            {
                seen = 1;
                break;
            }
            entry = entry->next;
        }

        if (seen)
        {
            result = current;
            break;
        }

        entry = malloc(sizeof(VisitedEntry));
        entry->node = current;
        entry->next = table[bucket];
        table[bucket] = entry;

        current = current->next;
    }

    for (i = 0; i < VISITED_TABLE_SIZE; i++)
    {
        VisitedEntry *entry = table[i];

        while (entry != NULL)
        {
            VisitedEntry *next = entry->next;

            free(entry);
            entry = next;
        }
    }

    return result;
}

int main()
{
    Node *head = createNode(1);
    Node *loopStart;

    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    loopStart = head->next;
    head->next->next->next->next = loopStart;

    printf("hasLoop = %d\n", hasLoop(head));
    printf("loop starts at value %d\n", findLoopStart(head)->value);
    printf("loop starts at value %d (via hash)\n", findLoopStartHash(head)->value);

    // Break the loop before freeing, otherwise freeList would never
    // terminate walking a cycle.
    head->next->next->next->next = NULL;
    free(head->next->next->next);
    free(head->next->next);
    free(head->next);
    free(head);

    return 0;
}
