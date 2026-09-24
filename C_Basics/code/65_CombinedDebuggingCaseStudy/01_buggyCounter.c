#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_COUNT 4
#define INCREMENTS_PER_THREAD 100000
#define RECORD_COUNT 4

// Two deliberately seeded, independent bugs for the NOTES.md walkthrough:
//
// 1. Heap corruption (buildRecords): an off-by-one heap buffer overflow -
//    writes RECORD_COUNT + 1 elements into a RECORD_COUNT-element heap
//    allocation. Found precisely with ASan.
// 2. Data race (incrementCounter): multiple threads increment a shared
//    counter without synchronization. Found precisely with TSan.
//
// The two bugs are isolated from each other so each can be triggered and
// diagnosed independently - see NOTES.md for the tool-by-tool walkthrough.

int sharedCounter = 0;

/*****************************************************************************
 * Name: buildRecords
 *
 * Description:
 *         Allocates and fills a heap array of ints. Contains a deliberate
 *         off-by-one heap buffer overflow: the loop writes one element past
 *         the end of the allocation.
 *
 * Returns:
 *         Pointer to a heap array (nominally RECORD_COUNT ints, but the
 *         write past its end corrupts adjacent heap memory).
 *****************************************************************************/
int *buildRecords(void)
{
    int *records = malloc(RECORD_COUNT * sizeof(int));
    int i;

    if (records == NULL)
    {
        return NULL;
    }

    // BUG: off-by-one - should be i < RECORD_COUNT. Writes one int past the
    // end of the allocation, corrupting whatever heap metadata or adjacent
    // allocation follows it.
    for (i = 0; i <= RECORD_COUNT; i++)
    {
        records[i] = i * i;
    }

    return records;
}

/*****************************************************************************
 * Name: incrementCounter
 *
 * Description:
 *         Thread entry point that increments the shared global counter many
 *         times with no synchronization - a deliberate data race.
 *
 * Inputs:
 *         arg : unused thread argument.
 *
 * Returns:
 *         NULL.
 *****************************************************************************/
void *incrementCounter(void *arg)
{
    int i;

    (void) arg;

    // BUG: read-modify-write on sharedCounter with no lock/atomic - a
    // classic unprotected-increment data race between threads.
    for (i = 0; i < INCREMENTS_PER_THREAD; i++)
    {
        sharedCounter++;
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[THREAD_COUNT];
    int *records;
    int i;

    records = buildRecords();
    printf("records[0] = %d\n", records[0]);
    free(records);

    for (i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&threads[i], NULL, incrementCounter, NULL);
    }
    for (i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Expected THREAD_COUNT * INCREMENTS_PER_THREAD; the race usually makes
    // this come out lower due to lost updates.
    printf("sharedCounter = %d (expected %d)\n", sharedCounter,
           THREAD_COUNT * INCREMENTS_PER_THREAD);

    return 0;
}
