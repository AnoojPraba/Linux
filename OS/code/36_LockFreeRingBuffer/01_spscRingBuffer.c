#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define BUFFER_SIZE 16
#define ITEMS_TO_PRODUCE 100000

// Single-producer/single-consumer ring buffer: because exactly one thread
// ever writes "tail" and exactly one thread ever writes "head", plain
// atomic loads/stores are enough - no compare-and-swap loop needed, unlike
// a true multi-producer/multi-consumer lock-free queue would require.
// This is the same head/tail circular-index idea as the mutex-protected
// bounded buffer in 14_SyncProblems/01_boundedBufferProducerConsumer.c,
// but with atomics standing in for the mutex + condition variables there.
typedef struct
{
    int data[BUFFER_SIZE];
    atomic_size_t head;
    atomic_size_t tail;
} RingBuffer;

void ringBufferInit(RingBuffer *rb)
{
    atomic_store(&rb->head, 0);
    atomic_store(&rb->tail, 0);
}

// Returns 0 on success, -1 if the buffer is full - the producer's own
// wait/retry policy (here, just busy-spin in the caller) is deliberately
// left outside this function.
int ringBufferPush(RingBuffer *rb, int value)
{
    size_t tail = atomic_load(&rb->tail);
    size_t nextTail = (tail + 1) % BUFFER_SIZE;

    if (nextTail == atomic_load(&rb->head))
    {
        return -1;
    }

    rb->data[tail] = value;
    // Only the producer ever writes tail - publishing the new tail value
    // after writing the data is what makes it safe for the consumer to
    // read data[tail] as soon as it observes the updated tail.
    atomic_store(&rb->tail, nextTail);
    return 0;
}

int ringBufferPop(RingBuffer *rb, int *outValue)
{
    size_t head = atomic_load(&rb->head);

    if (head == atomic_load(&rb->tail))
    {
        return -1;
    }

    *outValue = rb->data[head];
    atomic_store(&rb->head, (head + 1) % BUFFER_SIZE);
    return 0;
}

RingBuffer sharedBuffer;

void *producer(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < ITEMS_TO_PRODUCE; i++)
    {
        while (ringBufferPush(&sharedBuffer, i) != 0)
        {
            // Buffer full - busy-spin until the consumer makes room. A
            // real system would likely back off or yield here instead.
        }
    }
    return NULL;
}

void *consumer(void *arg)
{
    int consumed = 0;
    long checksum = 0;

    (void)arg;
    while (consumed < ITEMS_TO_PRODUCE)
    {
        int value;

        if (ringBufferPop(&sharedBuffer, &value) == 0)
        {
            checksum += value;
            consumed++;
        }
    }

    printf("consumed %d items, checksum = %ld\n", consumed, checksum);
    return NULL;
}

int main()
{
    pthread_t producerThread;
    pthread_t consumerThread;
    long expectedChecksum = ((long)(ITEMS_TO_PRODUCE - 1) * ITEMS_TO_PRODUCE) / 2;

    ringBufferInit(&sharedBuffer);

    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    printf("expected checksum = %ld\n", expectedChecksum);
    return 0;
}
