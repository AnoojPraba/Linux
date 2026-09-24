#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS   3
#define WRITER_DELAY  1

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int sharedValue = 0;

/*****************************************************************************
 * Name: reader
 *
 * Description:
 *         Takes the rwlock for reading and prints the shared value. Multiple
 *         readers can hold the read lock concurrently - no reader blocks
 *         another reader, only a writer blocks (and is blocked by) everyone.
 *
 * Inputs:
 *         arg : reader id, passed as a pointer-sized integer.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *reader(void *arg)
{
    long id = (long)arg;

    pthread_rwlock_rdlock(&rwlock);
    printf("reader %ld: sharedValue = %d\n", id, sharedValue);
    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

/*****************************************************************************
 * Name: writer
 *
 * Description:
 *         Takes the rwlock for writing and updates the shared value. A
 *         writer needs exclusive access - it blocks until no readers or
 *         other writers hold the lock, and blocks all of them while held.
 *
 * Inputs:
 *         arg : unused.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *writer(void *arg)
{
    (void)arg;

    sleep(WRITER_DELAY);
    pthread_rwlock_wrlock(&rwlock);
    sharedValue++;
    printf("writer: updated sharedValue to %d\n", sharedValue);
    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

int main()
{
    pthread_t readers[NUM_READERS];
    pthread_t writerThread;
    int i;

    for (i = 0; i < NUM_READERS; i++)
    {
        pthread_create(&readers[i], NULL, reader, (void *)(long)i);
    }
    pthread_create(&writerThread, NULL, writer, NULL);

    for (i = 0; i < NUM_READERS; i++)
    {
        pthread_join(readers[i], NULL);
    }
    pthread_join(writerThread, NULL);

    pthread_rwlock_destroy(&rwlock);
    return 0;
}
