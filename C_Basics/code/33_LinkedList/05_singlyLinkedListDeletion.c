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

    if (current == NULL)
    {
        printf("(empty)\n");
        return;
    }

    while (current != NULL)
    {
        printf("%d -> ", current->value);
        current = current->next;
    }
    printf("NULL\n");
}

// Deleting the head means the caller's pointer has to change, so a double
// pointer earns its keep here (per the project's double-pointer-avoidance
// rule, this is a case where there's genuinely no other way).
Node *deleteHead(Node *head)
{
    Node *oldHead = head;

    if (head == NULL)
    {
        return NULL;
    }

    head = head->next;
    free(oldHead);
    return head;
}

Node *deleteTail(Node *head)
{
    Node *current;

    if (head == NULL)
    {
        return NULL;
    }

    if (head->next == NULL)
    {
        free(head);
        return NULL;
    }

    current = head;
    while (current->next->next != NULL)
    {
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
    return head;
}

// Deleting by value must fix up the previous node's next pointer to skip
// over the removed node, or the list gets corrupted.
Node *deleteByValue(Node *head, int value)
{
    Node *current;
    Node *prev;

    if (head == NULL)
    {
        return NULL;
    }

    if (head->value == value)
    {
        return deleteHead(head);
    }

    prev = head;
    current = head->next;
    while (current != NULL)
    {
        if (current->value == value)
        {
            prev->next = current->next;
            free(current);
            return head;
        }
        prev = current;
        current = current->next;
    }
    return head;
}

#define INDEX_HEAD 0

Node *deleteByPosition(Node *head, int position)
{
    Node *current;
    Node *prev;
    int index;

    if ((head == NULL) || (position < INDEX_HEAD))
    {
        return head;
    }

    if (position == INDEX_HEAD)
    {
        return deleteHead(head);
    }

    prev = head;
    current = head->next;
    index = 1;
    while ((current != NULL) && (index != position))
    {
        prev = current;
        current = current->next;
        index++;
    }

    if (current == NULL)
    {
        return head;
    }

    prev->next = current->next;
    free(current);
    return head;
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
    append(head, 5);

    printf("initial:            ");
    printList(head);

    head = deleteByValue(head, 3);
    printf("after delete val 3: ");
    printList(head);

    head = deleteByPosition(head, 1);
    printf("after delete pos 1: ");
    printList(head);

    head = deleteHead(head);
    printf("after delete head:  ");
    printList(head);

    head = deleteTail(head);
    printf("after delete tail:  ");
    printList(head);

    // Only one node left - deleting it (as head, tail, or by value/position)
    // must leave the list empty rather than dangling.
    head = deleteHead(head);
    printf("after delete only:  ");
    printList(head);

    // Edge cases on an already-empty list must be safe no-ops.
    head = deleteHead(head);
    head = deleteTail(head);
    head = deleteByValue(head, 99);
    head = deleteByPosition(head, 0);
    printf("empty-list ops ok:  ");
    printList(head);

    freeList(head);
    return 0;
}
