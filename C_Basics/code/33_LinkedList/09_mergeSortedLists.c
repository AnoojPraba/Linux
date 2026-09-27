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

/*****************************************************************************
 * Name: mergeSortedLists
 *
 * Description:
 *         Merges two sorted singly linked lists into one sorted list, using
 *         a dummy head node to avoid special-casing which list contributes
 *         the new head. Walks both lists comparing their current nodes and
 *         always attaches the smaller one to the merged list's tail, then
 *         once one list is exhausted, attaches whatever remains of the
 *         other list wholesale (it's already sorted, so no more comparisons
 *         are needed).
 *
 * Inputs:
 *         a : head of the first sorted list.
 *         b : head of the second sorted list.
 *
 * Returns:
 *         Head of the merged sorted list.
 *****************************************************************************/
Node *mergeSortedLists(Node *a, Node *b)
{
    Node dummy;
    Node *tail;

    dummy.next = NULL;
    tail = &dummy;

    while ((a != NULL) && (b != NULL))
    {
        if (a->value <= b->value)
        {
            tail->next = a;
            a = a->next;
        }
        else
        {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }

    tail->next = (a != NULL) ? a : b;

    return dummy.next;
}

void printList(Node *head)
{
    while (head != NULL)
    {
        printf("%d ", head->value);
        head = head->next;
    }
    printf("\n");
}

void freeList(Node *head)
{
    Node *next;

    while (head != NULL)
    {
        next = head->next;
        free(head);
        head = next;
    }
}

int main(void)
{
    Node *a = createNode(1);
    Node *b = createNode(2);
    Node *merged;

    a->next = createNode(3);
    a->next->next = createNode(5);

    b->next = createNode(4);
    b->next->next = createNode(6);

    merged = mergeSortedLists(a, b);
    printf("merged: ");
    printList(merged);

    freeList(merged);
    return 0;
}
