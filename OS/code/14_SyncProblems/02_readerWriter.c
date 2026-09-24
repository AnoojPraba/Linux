#include <stdio.h>
#include <pthread.h>

#define NUM_READERS 3

int sharedData = 0;
int activeReaders = 0;

pthread_mutex_t readerCountLock = PTHREAD_MUTEX_INITIALIZER;
// Any number of readers may hold this "lock" simultaneously (tracked via
// activeReaders), but a writer needs it exclusively - the reader/writer
// problem is about allowing that extra concurrency safely, instead of
// treating every access like the plain mutual exclusion in
// 09_Threads/02_mutex.c.
pthread_mutex_t writeLock = PTHREAD_MUTEX_INITIALIZER;

void *reader(void *arg)
{
    long id = (long)arg;

    pthread_mutex_lock(&readerCountLock);
    activeReaders++;
    if (activeReaders == 1)
    {
        // The first reader blocks writers; later readers can proceed
        // without touching writeLock at all, letting many readers overlap.
        pthread_mutex_lock(&writeLock);
    }
    pthread_mutex_unlock(&readerCountLock);

    printf("reader %ld: read sharedData = %d\n", id, sharedData);

    pthread_mutex_lock(&readerCountLock);
    activeReaders--;
    if (activeReaders == 0)
    {
        // The last reader to leave releases writers again.
        pthread_mutex_unlock(&writeLock);
    }
    pthread_mutex_unlock(&readerCountLock);

    return NULL;
}

void *writer(void *arg)
{
    (void)arg;

    // A writer needs exclusive access - no reader and no other writer can
    // be active at the same time.
    pthread_mutex_lock(&writeLock);
    sharedData++;
    printf("writer: wrote sharedData = %d\n", sharedData);
    pthread_mutex_unlock(&writeLock);

    return NULL;
}

int main()
{
    pthread_t readers[NUM_READERS];
    pthread_t writerThread;
    int i;

    pthread_create(&writerThread, NULL, writer, NULL);
    pthread_join(writerThread, NULL);

    for (i = 0; i < NUM_READERS; i++)
    {
        pthread_create(&readers[i], NULL, reader, (void *)(long)i);
    }
    for (i = 0; i < NUM_READERS; i++)
    {
        pthread_join(readers[i], NULL);
    }

    pthread_mutex_destroy(&readerCountLock);
    pthread_mutex_destroy(&writeLock);
    return 0;
}
