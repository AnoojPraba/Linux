#include <stdio.h>

#define QUEUE_CAPACITY 5

// A count field disambiguates full vs empty (both would otherwise have
// front == rear), avoiding the classic circular-queue off-by-one trap.
typedef struct
{
    int data[QUEUE_CAPACITY];
    int front;
    int rear;
    int count;
} ArrayQueue;

/*****************************************************************************
 * Name: queueInit
 *
 * Description:
 *         Initializes an array-based circular queue to empty.
 *
 * Inputs:
 *         queue : queue to initialize.
 *
 * Returns:
 *         None
 *****************************************************************************/
void queueInit(ArrayQueue *queue)
{
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
}

/*****************************************************************************
 * Name: queueIsEmpty
 *
 * Description:
 *         Checks whether the queue holds no elements.
 *
 * Inputs:
 *         queue : queue to check.
 *
 * Returns:
 *         1 if empty, 0 otherwise.
 *****************************************************************************/
int queueIsEmpty(const ArrayQueue *queue)
{
    return queue->count == 0;
}

/*****************************************************************************
 * Name: queueIsFull
 *
 * Description:
 *         Checks whether the queue has reached its fixed capacity.
 *
 * Inputs:
 *         queue : queue to check.
 *
 * Returns:
 *         1 if full, 0 otherwise.
 *****************************************************************************/
int queueIsFull(const ArrayQueue *queue)
{
    return queue->count == QUEUE_CAPACITY;
}

/*****************************************************************************
 * Name: queueEnqueue
 *
 * Description:
 *         Inserts a value at the rear of the queue, wrapping the rear index
 *         modulo the capacity.
 *
 * Inputs:
 *         queue : queue to insert into.
 *         value : value to insert.
 *
 * Returns:
 *         1 on success, 0 if the queue was full.
 *****************************************************************************/
int queueEnqueue(ArrayQueue *queue, int value)
{
    if (queueIsFull(queue))
    {
        return 0;
    }
    queue->data[queue->rear] = value;
    queue->rear = (queue->rear + 1) % QUEUE_CAPACITY;
    queue->count = queue->count + 1;
    return 1;
}

/*****************************************************************************
 * Name: queueDequeue
 *
 * Description:
 *         Removes and returns the value at the front of the queue, wrapping
 *         the front index modulo the capacity.
 *
 * Inputs:
 *         queue : queue to remove from.
 *         value : out-param receiving the dequeued value.
 *
 * Returns:
 *         1 on success, 0 if the queue was empty.
 *****************************************************************************/
int queueDequeue(ArrayQueue *queue, int *value)
{
    if (queueIsEmpty(queue))
    {
        return 0;
    }
    *value = queue->data[queue->front];
    queue->front = (queue->front + 1) % QUEUE_CAPACITY;
    queue->count = queue->count - 1;
    return 1;
}

int main(void)
{
    ArrayQueue queue;
    int value;
    int i;

    queueInit(&queue);
    for (i = 1; i <= QUEUE_CAPACITY; i++)
    {
        queueEnqueue(&queue, i * 10);
    }

    queueDequeue(&queue, &value);
    printf("dequeued %d\n", value);
    queueEnqueue(&queue, 999);

    while (queueDequeue(&queue, &value))
    {
        printf("dequeued %d\n", value);
    }

    return 0;
}
