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
 * Name: findMiddle
 *
 * Description:
 *         Finds the middle node of a singly linked list using the
 *         slow/fast pointer technique: slow advances one node per step,
 *         fast advances two. When fast reaches the end, slow sits at the
 *         middle. For an even-length list there are two middle nodes; this
 *         implementation returns the SECOND of the two (the common
 *         convention), because fast becomes NULL (not fast->next == NULL)
 *         exactly when slow has taken one extra step past the first
 *         middle. This is a different use of slow/fast pointers than
 *         cycle detection (04_loopFinder.c) - cycle detection watches for
 *         slow == fast to detect a loop, while this uses the relative
 *         speed difference (fast covers ground twice as fast) to locate
 *         the midpoint, and never expects the two pointers to meet.
 *
 * Inputs:
 *         head : head of the list.
 *
 * Returns:
 *         Pointer to the middle node (the second of two middles for an
 *         even-length list), or NULL if the list is empty.
 *****************************************************************************/
Node *findMiddle(Node *head)
{
    Node *slow = head;
    Node *fast = head;

    while ((fast != NULL) && (fast->next != NULL))
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
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
    Node *oddHead = createNode(1);
    Node *evenHead = createNode(1);
    Node *middle;

    oddHead->next = createNode(2);
    oddHead->next->next = createNode(3);
    oddHead->next->next->next = createNode(4);
    oddHead->next->next->next->next = createNode(5);

    evenHead->next = createNode(2);
    evenHead->next->next = createNode(3);
    evenHead->next->next->next = createNode(4);

    middle = findMiddle(oddHead);
    printf("middle of odd-length list = %d\n", middle->value);

    middle = findMiddle(evenHead);
    printf("middle of even-length list = %d\n", middle->value);

    freeList(oddHead);
    freeList(evenHead);
    return 0;
}
