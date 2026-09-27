#include <stdio.h>

#define RING_BUFFER_CAPACITY 8

// Simulates hardware-shared state: in real firmware, writeIndex/readIndex
// would be declared volatile since they are written from interrupt context
// (bufferPush, called from the ISR) and read from main-loop context
// (bufferPop), or vice versa - see ../../OS/code/11_VolatileVsAtomicEmbedded
// for why the compiler must not cache/reorder accesses to such variables.
typedef struct
{
    unsigned char data[RING_BUFFER_CAPACITY];
    int writeIndex;
    int readIndex;
    int count;
} RingBuffer;

/*****************************************************************************
 * Name: ringBufferInit
 *
 * Description:
 *         Initializes a ring buffer to empty.
 *
 * Inputs:
 *         ring : ring buffer to initialize.
 *
 * Returns:
 *         None
 *****************************************************************************/
void ringBufferInit(RingBuffer *ring)
{
    ring->writeIndex = 0;
    ring->readIndex = 0;
    ring->count = 0;
}

/*****************************************************************************
 * Name: bufferPush
 *
 * Description:
 *         Called from the "ISR" (here a plain function call standing in for
 *         a real hardware interrupt, since a real interrupt isn't available
 *         to simulate directly). Must be fast and non-blocking: it only
 *         writes the incoming byte and advances the write index, wrapping
 *         around at capacity, using the same count-based full/empty
 *         detection already used by 03_arrayQueue.c in this folder. It does
 *         NOT process the byte - real processing happens later in the main
 *         loop via bufferPop, so the interrupt handler returns as quickly
 *         as possible.
 *
 * Inputs:
 *         ring  : ring buffer to push into.
 *         value : incoming byte to store.
 *
 * Returns:
 *         1 on success, 0 if the buffer was full (byte dropped).
 *****************************************************************************/
int bufferPush(RingBuffer *ring, unsigned char value)
{
    if (ring->count == RING_BUFFER_CAPACITY)
    {
        return 0;
    }
    ring->data[ring->writeIndex] = value;
    ring->writeIndex = (ring->writeIndex + 1) % RING_BUFFER_CAPACITY;
    ring->count = ring->count + 1;
    return 1;
}

/*****************************************************************************
 * Name: bufferPop
 *
 * Description:
 *         Called from the main loop to drain one byte pushed by the "ISR".
 *         Reads the byte at the read index and advances it, wrapping around
 *         at capacity.
 *
 * Inputs:
 *         ring  : ring buffer to pop from.
 *         value : out-param receiving the popped byte.
 *
 * Returns:
 *         1 on success, 0 if the buffer was empty.
 *****************************************************************************/
int bufferPop(RingBuffer *ring, unsigned char *value)
{
    if (ring->count == 0)
    {
        return 0;
    }
    *value = ring->data[ring->readIndex];
    ring->readIndex = (ring->readIndex + 1) % RING_BUFFER_CAPACITY;
    ring->count = ring->count - 1;
    return 1;
}

int main(void)
{
    RingBuffer uartRx;
    unsigned char incoming[] = "UART!";
    unsigned char popped;
    int i;

    ringBufferInit(&uartRx);

    // Simulate the ISR firing once per incoming byte.
    for (i = 0; incoming[i] != '\0'; i++)
    {
        bufferPush(&uartRx, incoming[i]);
    }

    // Simulate the main loop draining the buffer at its own pace.
    while (bufferPop(&uartRx, &popped))
    {
        printf("main loop processed byte: '%c'\n", popped);
    }

    return 0;
}
