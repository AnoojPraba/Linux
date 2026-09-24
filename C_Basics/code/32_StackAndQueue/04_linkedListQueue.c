#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode
{
    int value;
    struct QueueNode *next;
} QueueNode;

// head/tail pointers give O(1) enqueue (append at tail) and O(1) dequeue
// (remove at head), unlike a singly linked list without a tail pointer.
typedef struct
{
    QueueNode *head;
    QueueNode *tail;
} LinkedQueue;

/*****************************************************************************
 * Name: queueInit
 *
 * Description:
 *         Initializes a linked-list-based queue to empty.
 *
 * Inputs:
 *         queue : queue to initialize.
 *
 * Returns:
 *         None
 *****************************************************************************/
void queueInit(LinkedQueue *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
}

/*****************************************************************************
 * Name: queueEnqueue
 *
 * Description:
 *         Appends a value at the tail of the linked-list queue.
 *
 * Inputs:
 *         queue : queue to insert into.
 *         value : value to insert.
 *
 * Returns:
 *         None
 *****************************************************************************/
void queueEnqueue(LinkedQueue *queue, int value)
{
    QueueNode *node = malloc(sizeof(QueueNode));

    node->value = value;
    node->next = NULL;
    if (queue->tail == NULL)
    {
        queue->head = node;
        queue->tail = node;
    }
    else
    {
        queue->tail->next = node;
        queue->tail = node;
    }
}

/*****************************************************************************
 * Name: queueDequeue
 *
 * Description:
 *         Removes and returns the value at the head of the linked-list
 *         queue, freeing the removed node.
 *
 * Inputs:
 *         queue : queue to remove from.
 *         value : out-param receiving the dequeued value.
 *
 * Returns:
 *         1 on success, 0 if the queue was empty.
 *****************************************************************************/
int queueDequeue(LinkedQueue *queue, int *value)
{
    QueueNode *node = queue->head;

    if (node == NULL)
    {
        return 0;
    }
    *value = node->value;
    queue->head = node->next;
    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }
    free(node);
    return 1;
}

int main(void)
{
    LinkedQueue queue;
    int value;

    queueInit(&queue);
    queueEnqueue(&queue, 1);
    queueEnqueue(&queue, 2);
    queueEnqueue(&queue, 3);

    while (queueDequeue(&queue, &value))
    {
        printf("dequeued %d\n", value);
    }

    return 0;
}
