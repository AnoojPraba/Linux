#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define BUFFER_SIZE 16
#define NUM_PRODUCER_THREADS 4
#define NUM_CONSUMER_THREADS 4
#define ITEMS_PER_PRODUCER 25000

// Vyukov-style bounded lock-free MPMC queue. Unlike the SPSC ring buffer in
// 01_spscRingBuffer.c (single owner of each index, no CAS needed), multiple
// producers race on the same write index and multiple consumers race on the
// same read index, so both must be advanced with a compare_exchange_weak
// loop. Each slot carries its own sequence number to tell producers/
// consumers whether that slot is currently ready for them, and to handle
// the ring wrapping around ("lapping") safely.
typedef struct
{
    atomic_size_t sequence;
    int value;
} Slot;

typedef struct
{
    Slot slots[BUFFER_SIZE];
    atomic_size_t writeIndex;
    atomic_size_t readIndex;
} MpmcRingBuffer;

/*****************************************************************************
 * Name: mpmcRingBufferInit
 *
 * Description:
 *         Initializes every slot's sequence number to its own index (so
 *         slot N starts "ready for write" by the first producer to reach
 *         write-index N) and resets both shared indices to zero.
 *
 * Inputs:
 *         rb : the ring buffer to initialize.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void mpmcRingBufferInit(MpmcRingBuffer *rb)
{
    size_t i;

    for (i = 0; i < BUFFER_SIZE; i++)
    {
        atomic_store(&rb->slots[i].sequence, i);
    }
    atomic_store(&rb->writeIndex, 0);
    atomic_store(&rb->readIndex, 0);
}

/*****************************************************************************
 * Name: mpmcRingBufferPush
 *
 * Description:
 *         Attempts to claim the next slot at the current write index via a
 *         CAS loop: a slot is ready for write when its sequence equals the
 *         write index we are trying to claim. On success, writes the data
 *         and publishes the slot as "ready for read" by bumping its
 *         sequence to writeIndex + 1.
 *
 * Inputs:
 *         rb    : the ring buffer to push into.
 *         value : the value to enqueue.
 *
 * Returns:
 *         0 on success, -1 if the buffer is full.
 *****************************************************************************/
int mpmcRingBufferPush(MpmcRingBuffer *rb, int value)
{
    size_t pos = atomic_load_explicit(&rb->writeIndex, memory_order_relaxed);

    for (;;)
    {
        Slot *slot = &rb->slots[pos % BUFFER_SIZE];
        size_t seq = atomic_load_explicit(&slot->sequence, memory_order_acquire);
        long diff = (long)seq - (long)pos;

        if (diff == 0)
        {
            if (atomic_compare_exchange_weak_explicit(&rb->writeIndex, &pos, pos + 1,
                                                        memory_order_relaxed,
                                                        memory_order_relaxed))
            {
                slot->value = value;
                atomic_store_explicit(&slot->sequence, pos + 1, memory_order_release);
                return 0;
            }
        }
        else if (diff < 0)
        {
            return -1;
        }
        else
        {
            pos = atomic_load_explicit(&rb->writeIndex, memory_order_relaxed);
        }
    }
}

/*****************************************************************************
 * Name: mpmcRingBufferPop
 *
 * Description:
 *         Attempts to claim the next slot at the current read index via a
 *         CAS loop: a slot is ready for read when its sequence equals
 *         readIndex + 1. On success, reads the data and publishes the slot
 *         as "ready for write" for a future lap by setting its sequence to
 *         readIndex + BUFFER_SIZE.
 *
 * Inputs:
 *         rb        : the ring buffer to pop from.
 *         outValue  : receives the dequeued value on success.
 *
 * Returns:
 *         0 on success, -1 if the buffer is empty.
 *****************************************************************************/
int mpmcRingBufferPop(MpmcRingBuffer *rb, int *outValue)
{
    size_t pos = atomic_load_explicit(&rb->readIndex, memory_order_relaxed);

    for (;;)
    {
        Slot *slot = &rb->slots[pos % BUFFER_SIZE];
        size_t seq = atomic_load_explicit(&slot->sequence, memory_order_acquire);
        long diff = (long)seq - (long)(pos + 1);

        if (diff == 0)
        {
            if (atomic_compare_exchange_weak_explicit(&rb->readIndex, &pos, pos + 1,
                                                        memory_order_relaxed,
                                                        memory_order_relaxed))
            {
                *outValue = slot->value;
                atomic_store_explicit(&slot->sequence, pos + BUFFER_SIZE,
                                       memory_order_release);
                return 0;
            }
        }
        else if (diff < 0)
        {
            return -1;
        }
        else
        {
            pos = atomic_load_explicit(&rb->readIndex, memory_order_relaxed);
        }
    }
}

MpmcRingBuffer sharedBuffer;
atomic_long consumedChecksum;
atomic_int totalConsumed;

/*****************************************************************************
 * Name: producerThread
 *
 * Description:
 *         Pushes ITEMS_PER_PRODUCER values into the shared ring buffer,
 *         busy-spinning on push failures (buffer full).
 *
 * Inputs:
 *         arg : unused.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *producerThread(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < ITEMS_PER_PRODUCER; i++)
    {
        while (mpmcRingBufferPush(&sharedBuffer, i) != 0)
        {
            // Buffer full - busy-spin until a consumer makes room.
        }
    }
    return NULL;
}

/*****************************************************************************
 * Name: consumerThread
 *
 * Description:
 *         Pops values from the shared ring buffer until the total expected
 *         item count across all producers has been consumed, accumulating
 *         a running checksum for validation.
 *
 * Inputs:
 *         arg : unused.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *consumerThread(void *arg)
{
    int totalItems = NUM_PRODUCER_THREADS * ITEMS_PER_PRODUCER;

    (void)arg;
    for (;;)
    {
        int value;

        if (atomic_load(&totalConsumed) >= totalItems)
        {
            break;
        }
        if (mpmcRingBufferPop(&sharedBuffer, &value) == 0)
        {
            atomic_fetch_add(&consumedChecksum, value);
            atomic_fetch_add(&totalConsumed, 1);
        }
    }
    return NULL;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Runs multiple producer and consumer threads against one shared
 *         MPMC ring buffer, then reports the consumed count and checksum
 *         against the expected values.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    pthread_t producers[NUM_PRODUCER_THREADS];
    pthread_t consumers[NUM_CONSUMER_THREADS];
    long expectedChecksum = (long)NUM_PRODUCER_THREADS *
                             ((long)(ITEMS_PER_PRODUCER - 1) * ITEMS_PER_PRODUCER) / 2;
    int i;

    mpmcRingBufferInit(&sharedBuffer);
    atomic_store(&consumedChecksum, 0);
    atomic_store(&totalConsumed, 0);

    for (i = 0; i < NUM_CONSUMER_THREADS; i++)
    {
        pthread_create(&consumers[i], NULL, consumerThread, NULL);
    }
    for (i = 0; i < NUM_PRODUCER_THREADS; i++)
    {
        pthread_create(&producers[i], NULL, producerThread, NULL);
    }

    for (i = 0; i < NUM_PRODUCER_THREADS; i++)
    {
        pthread_join(producers[i], NULL);
    }
    for (i = 0; i < NUM_CONSUMER_THREADS; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    printf("consumed %d items, checksum = %ld\n", atomic_load(&totalConsumed),
           atomic_load(&consumedChecksum));
    printf("expected checksum = %ld\n", expectedChecksum);
    return 0;
}
