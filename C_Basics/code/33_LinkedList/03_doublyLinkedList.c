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
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void append(DNode *head, int value)
{
    DNode *current = head;
    DNode *node = createNode(value);

    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = node;
    node->prev = current;
}

void printForward(DNode *head)
{
    DNode *current = head;

    while (current != NULL)
    {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
}

// The prev pointer is what makes backward traversal possible without
// restarting from head, unlike a singly linked list.
void printBackward(DNode *tail)
{
    DNode *current = tail;

    while (current != NULL)
    {
        printf("%d ", current->value);
        current = current->prev;
    }
    printf("\n");
}

void freeList(DNode *head)
{
    DNode *current = head;

    while (current != NULL)
    {
        DNode *next = current->next;

        free(current);
        current = next;
    }
}

int main()
{
    DNode *head = createNode(1);
    DNode *tail;

    append(head, 2);
    append(head, 3);
    append(head, 4);

    tail = head;
    while (tail->next != NULL)
    {
        tail = tail->next;
    }

    printf("forward:  ");
    printForward(head);
    printf("backward: ");
    printBackward(tail);

    freeList(head);
    return 0;
}
