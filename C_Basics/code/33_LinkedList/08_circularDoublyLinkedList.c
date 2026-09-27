#include <stdio.h>
#include <stdlib.h>

typedef struct DNode
{
    int value;
    struct DNode *prev;
    struct DNode *next;
} DNode;

DNode *createNode(int value)
{
    DNode *node = malloc(sizeof(DNode));

    node->value = value;
    node->prev = node;
    node->next = node;
    return node;
}

// Insert at tail, i.e. just before head, keeping both prev/next links
// circular. Only returns a different head when the list starts empty.
DNode *insertAtTail(DNode *head, int value)
{
    DNode *node;
    DNode *tail;

    if (head == NULL)
    {
        return createNode(value);
    }

    node = createNode(value);
    tail = head->prev;

    tail->next = node;
    node->prev = tail;
    node->next = head;
    head->prev = node;
    return head;
}

#define WRAP_COUNT 2

void printForwardWithWraparound(DNode *head, int listSize)
{
    DNode *current;
    int totalSteps;
    int i;

    if (head == NULL)
    {
        printf("(empty)\n");
        return;
    }

    current = head;
    totalSteps = WRAP_COUNT * listSize;
    for (i = 0; i < totalSteps; i++)
    {
        printf("%d -> ", current->value);
        current = current->next;
        if ((current == head) && (i != totalSteps - 1))
        {
            printf("[wrapped back to head] ");
        }
    }
    printf("...\n");
}

void printBackwardWithWraparound(DNode *head, int listSize)
{
    DNode *tail;
    DNode *current;
    int totalSteps;
    int i;

    if (head == NULL)
    {
        printf("(empty)\n");
        return;
    }

    tail = head->prev;
    current = tail;
    totalSteps = WRAP_COUNT * listSize;
    for (i = 0; i < totalSteps; i++)
    {
        printf("%d -> ", current->value);
        current = current->prev;
        if ((current == tail) && (i != totalSteps - 1))
        {
            printf("[wrapped back to tail] ");
        }
    }
    printf("...\n");
}

int listSize(DNode *head)
{
    DNode *current;
    int count;

    if (head == NULL)
    {
        return 0;
    }

    count = 1;
    current = head->next;
    while (current != head)
    {
        count++;
        current = current->next;
    }
    return count;
}

// Deleting a node in a circular doubly linked list still has to patch both
// neighbors, same as the non-circular case, but never hits a NULL prev/next
// - the only special case is deleting the last remaining node, where the
// node points to itself and the result is an empty list.
DNode *deleteNode(DNode *head, int value)
{
    DNode *current = head;

    if (head == NULL)
    {
        return NULL;
    }

    do
    {
        if (current->value == value)
        {
            if (current->next == current)
            {
                free(current);
                return NULL;
            }

            current->prev->next = current->next;
            current->next->prev = current->prev;

            if (current == head)
            {
                head = current->next;
            }

            free(current);
            return head;
        }
        current = current->next;
    } while (current != head);

    return head;
}

void freeCircularList(DNode *head)
{
    DNode *current;

    if (head == NULL)
    {
        return;
    }

    current = head->next;
    while (current != head)
    {
        DNode *next = current->next;

        free(current);
        current = next;
    }
    free(head);
}

int main()
{
    DNode *head = NULL;
    int size;

    head = insertAtTail(head, 1);
    head = insertAtTail(head, 2);
    head = insertAtTail(head, 3);
    head = insertAtTail(head, 4);

    size = listSize(head);
    printf("list size: %d\n", size);

    printf("forward (%d*size steps):  ", WRAP_COUNT);
    printForwardWithWraparound(head, size);
    printf("backward (%d*size steps): ", WRAP_COUNT);
    printBackwardWithWraparound(head, size);

    head = deleteNode(head, 3);
    size = listSize(head);
    printf("after delete val 3, size %d, forward:  ", size);
    printForwardWithWraparound(head, size);

    head = deleteNode(head, 1);
    size = listSize(head);
    printf("after delete head val 1, size %d, forward: ", size);
    printForwardWithWraparound(head, size);

    freeCircularList(head);
    return 0;
}
