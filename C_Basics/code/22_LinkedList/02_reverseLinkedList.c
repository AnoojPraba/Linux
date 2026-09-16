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

// Reversing needs the new head handed back, so a double pointer earns its
// keep here (per the project's double-pointer-avoidance rule, this is the
// case where there's genuinely no other way).
Node *reverseList(Node *head)
{
    Node *prev = NULL;
    Node *current = head;

    while (current != NULL)
    {
        Node *next = current->next;

        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
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

    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);

    printf("before: ");
    printList(head);

    head = reverseList(head);

    printf("after:  ");
    printList(head);

    freeList(head);
    return 0;
}
