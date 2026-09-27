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

DNode *findTail(DNode *head)
{
    DNode *current = head;

    if (current == NULL)
    {
        return NULL;
    }

    while (current->next != NULL)
    {
        current = current->next;
    }
    return current;
}

void printForward(DNode *head)
{
    DNode *current = head;

    if (current == NULL)
    {
        printf("(empty)\n");
        return;
    }

    while (current != NULL)
    {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
}

void printBackward(DNode *tail)
{
    DNode *current = tail;

    if (current == NULL)
    {
        printf("(empty)\n");
        return;
    }

    while (current != NULL)
    {
        printf("%d ", current->value);
        current = current->prev;
    }
    printf("\n");
}

// Unlike a singly linked list, unlinking a doubly linked node has to patch
// up both its neighbors: the previous node's next and the next node's prev.
// Deleting the head changes what the caller's pointer refers to, so a
// double pointer earns its keep here (per the project's double-pointer-
// avoidance rule, this is a case where there's genuinely no other way).
DNode *deleteHead(DNode *head)
{
    DNode *oldHead = head;

    if (head == NULL)
    {
        return NULL;
    }

    head = head->next;
    if (head != NULL)
    {
        head->prev = NULL;
    }
    free(oldHead);
    return head;
}

DNode *deleteTail(DNode *head)
{
    DNode *tail = findTail(head);
    DNode *newTail;

    if (tail == NULL)
    {
        return NULL;
    }

    if (tail->prev == NULL)
    {
        free(tail);
        return NULL;
    }

    newTail = tail->prev;
    newTail->next = NULL;
    free(tail);
    return head;
}

DNode *deleteByValue(DNode *head, int value)
{
    DNode *current = head;

    while (current != NULL)
    {
        if (current->value == value)
        {
            if (current->prev != NULL)
            {
                current->prev->next = current->next;
            }
            else
            {
                head = current->next;
            }

            if (current->next != NULL)
            {
                current->next->prev = current->prev;
            }

            free(current);
            return head;
        }
        current = current->next;
    }
    return head;
}

#define INDEX_HEAD 0

DNode *deleteByPosition(DNode *head, int position)
{
    DNode *current = head;
    int index = INDEX_HEAD;

    if (position < INDEX_HEAD)
    {
        return head;
    }

    while ((current != NULL) && (index != position))
    {
        current = current->next;
        index++;
    }

    if (current == NULL)
    {
        return head;
    }

    if (current->prev != NULL)
    {
        current->prev->next = current->next;
    }
    else
    {
        head = current->next;
    }

    if (current->next != NULL)
    {
        current->next->prev = current->prev;
    }

    free(current);
    return head;
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

    append(head, 2);
    append(head, 3);
    append(head, 4);
    append(head, 5);

    printf("initial forward:            ");
    printForward(head);
    printf("initial backward:           ");
    printBackward(findTail(head));

    head = deleteByValue(head, 3);
    printf("after delete val 3 fwd:      ");
    printForward(head);
    printf("after delete val 3 bwd:      ");
    printBackward(findTail(head));

    head = deleteByPosition(head, 1);
    printf("after delete pos 1 fwd:      ");
    printForward(head);
    printf("after delete pos 1 bwd:      ");
    printBackward(findTail(head));

    head = deleteHead(head);
    printf("after delete head fwd:       ");
    printForward(head);
    printf("after delete head bwd:       ");
    printBackward(findTail(head));

    head = deleteTail(head);
    printf("after delete tail fwd:       ");
    printForward(head);
    printf("after delete tail bwd:       ");
    printBackward(findTail(head));

    // Only one node left - deleting it must leave the list empty, not
    // dangling, in both directions.
    head = deleteHead(head);
    printf("after delete only fwd:       ");
    printForward(head);
    printf("after delete only bwd:       ");
    printBackward(findTail(head));

    // Edge cases on an already-empty list must be safe no-ops.
    head = deleteHead(head);
    head = deleteTail(head);
    head = deleteByValue(head, 99);
    head = deleteByPosition(head, 0);
    printf("empty-list ops ok fwd:       ");
    printForward(head);

    freeList(head);
    return 0;
}
