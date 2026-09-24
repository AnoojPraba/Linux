#include <stdio.h>

#define DEQUE_CAPACITY 5

// Array-based circular deque: both ends move via modulo wraparound, and a
// count field (as in 03_arrayQueue) distinguishes full from empty.
typedef struct
{
    int data[DEQUE_CAPACITY];
    int front;
    int rear;
    int count;
} Deque;

/*****************************************************************************
 * Name: dequeInit
 *
 * Description:
 *         Initializes a circular deque to empty.
 *
 * Inputs:
 *         deque : deque to initialize.
 *
 * Returns:
 *         None
 *****************************************************************************/
void dequeInit(Deque *deque)
{
    deque->front = 0;
    deque->rear = 0;
    deque->count = 0;
}

/*****************************************************************************
 * Name: dequePushFront
 *
 * Description:
 *         Inserts a value at the front of the deque.
 *
 * Inputs:
 *         deque : deque to insert into.
 *         value : value to insert.
 *
 * Returns:
 *         1 on success, 0 if the deque was full.
 *****************************************************************************/
int dequePushFront(Deque *deque, int value)
{
    if (deque->count == DEQUE_CAPACITY)
    {
        return 0;
    }
    deque->front = (deque->front - 1 + DEQUE_CAPACITY) % DEQUE_CAPACITY;
    deque->data[deque->front] = value;
    deque->count = deque->count + 1;
    return 1;
}

/*****************************************************************************
 * Name: dequePushBack
 *
 * Description:
 *         Inserts a value at the back of the deque.
 *
 * Inputs:
 *         deque : deque to insert into.
 *         value : value to insert.
 *
 * Returns:
 *         1 on success, 0 if the deque was full.
 *****************************************************************************/
int dequePushBack(Deque *deque, int value)
{
    if (deque->count == DEQUE_CAPACITY)
    {
        return 0;
    }
    deque->data[deque->rear] = value;
    deque->rear = (deque->rear + 1) % DEQUE_CAPACITY;
    deque->count = deque->count + 1;
    return 1;
}

/*****************************************************************************
 * Name: dequePopFront
 *
 * Description:
 *         Removes and returns the value at the front of the deque.
 *
 * Inputs:
 *         deque : deque to remove from.
 *         value : out-param receiving the removed value.
 *
 * Returns:
 *         1 on success, 0 if the deque was empty.
 *****************************************************************************/
int dequePopFront(Deque *deque, int *value)
{
    if (deque->count == 0)
    {
        return 0;
    }
    *value = deque->data[deque->front];
    deque->front = (deque->front + 1) % DEQUE_CAPACITY;
    deque->count = deque->count - 1;
    return 1;
}

/*****************************************************************************
 * Name: dequePopBack
 *
 * Description:
 *         Removes and returns the value at the back of the deque.
 *
 * Inputs:
 *         deque : deque to remove from.
 *         value : out-param receiving the removed value.
 *
 * Returns:
 *         1 on success, 0 if the deque was empty.
 *****************************************************************************/
int dequePopBack(Deque *deque, int *value)
{
    if (deque->count == 0)
    {
        return 0;
    }
    deque->rear = (deque->rear - 1 + DEQUE_CAPACITY) % DEQUE_CAPACITY;
    *value = deque->data[deque->rear];
    deque->count = deque->count - 1;
    return 1;
}

int main(void)
{
    Deque deque;
    int value;

    dequeInit(&deque);
    dequePushBack(&deque, 2);
    dequePushBack(&deque, 3);
    dequePushFront(&deque, 1);

    dequePopFront(&deque, &value);
    printf("popped front %d\n", value);
    dequePopBack(&deque, &value);
    printf("popped back %d\n", value);

    return 0;
}
