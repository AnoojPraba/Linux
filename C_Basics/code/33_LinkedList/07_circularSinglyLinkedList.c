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
    node->next = node;
    return node;
}

// A circular list's tail always points back to head instead of NULL, so
// insertAtTail needs the head handed back only when the list starts empty
// (head == NULL) and must be created; otherwise the existing head is reused.
Node *insertAtTail(Node *head, int value)
{
    Node *node;
    Node *current;

    if (head == NULL)
    {
        return createNode(value);
    }

    node = createNode(value);
    current = head;
    while (current->next != head)
    {
        current = current->next;
    }
    current->next = node;
    node->next = head;
    return head;
}

#define WRAP_COUNT 2

// Traverses N*list_size steps to make the wraparound explicit: the walk
// keeps going past the last node back to head instead of stopping at NULL,
// which is the defining behavior of a circular list.
void printWithWraparound(Node *head, int listSize)
{
    Node *current;
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

// A list is genuinely circular only if walking from head eventually returns
// to head again via next pointers, rather than hitting NULL.
int isCircular(Node *head)
{
    Node *current;

    if (head == NULL)
    {
        return 0;
    }

    current = head->next;
    while ((current != head) && (current != NULL))
    {
        current = current->next;
    }
    return current == head;
}

int listSize(Node *head)
{
    Node *current;
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

void freeCircularList(Node *head)
{
    Node *current;

    if (head == NULL)
    {
        return;
    }

    current = head->next;
    while (current != head)
    {
        Node *next = current->next;

        free(current);
        current = next;
    }
    free(head);
}

int main()
{
    Node *head = NULL;
    int size;

    head = insertAtTail(head, 1);
    head = insertAtTail(head, 2);
    head = insertAtTail(head, 3);
    head = insertAtTail(head, 4);

    size = listSize(head);
    printf("list size: %d\n", size);
    printf("is circular: %d\n", isCircular(head));

    printf("traversal (%d*size steps): ", WRAP_COUNT);
    printWithWraparound(head, size);

    freeCircularList(head);
    return 0;
}
