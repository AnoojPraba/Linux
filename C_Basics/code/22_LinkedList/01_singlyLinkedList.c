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

// Appending needs the head passed by reference (double pointer) only when
// the list is empty and head itself must be reassigned; here head is always
// non-NULL by the time append is called, so a plain Node * traversal works.
void append(Node *head, int value)
{
    Node *current = head;

    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = createNode(value);
}

void printList(Node *head)
{
    Node *current = head;

    while (current != NULL)
    {
        printf("%d -> ", current->value);
        current = current->next;
    }
    printf("NULL\n");
}

void freeList(Node *head)
{
    Node *current = head;

    while (current != NULL)
    {
        Node *next = current->next;

        free(current);
        current = next;
    }
}

int main()
{
    Node *head = createNode(1);

    append(head, 2);
    append(head, 3);
    append(head, 4);

    printList(head);
    freeList(head);

    return 0;
}
