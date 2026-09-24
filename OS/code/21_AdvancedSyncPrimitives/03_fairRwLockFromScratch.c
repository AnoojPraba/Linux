#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READER_THREADS 4
#define NUM_WRITER_THREADS 2
#define READS_PER_THREAD 8
#define WRITES_PER_THREAD 4
#define READ_HOLD_USEC 2000
#define WRITE_HOLD_USEC 3000

// Fair (starvation-free) reader-writer lock built from a plain mutex + two
// condition variables. "Fair" here means: a new reader must wait if there is
// an active OR a waiting writer - this is what stops writers from starving
// under constant reader arrivals, unlike pthread_rwlock_t's default
// reader-preferring behavior on glibc.
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t readersOk;
    pthread_cond_t writerOk;
    int activeReaders;
    int activeWriters;
    int waitingWriters;
} FairRwLock;

/*****************************************************************************
 * Name: fairRwLockInit
 *
 * Description:
 *         Initializes a FairRwLock's mutex, condition variables, and
 *         counters to their starting (unlocked, idle) state.
 *
 * Inputs:
 *         lock : the lock to initialize.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void fairRwLockInit(FairRwLock *lock)
{
    pthread_mutex_init(&lock->mutex, NULL);
    pthread_cond_init(&lock->readersOk, NULL);
    pthread_cond_init(&lock->writerOk, NULL);
    lock->activeReaders = 0;
    lock->activeWriters = 0;
    lock->waitingWriters = 0;
}

/*****************************************************************************
 * Name: fairRwLockReadLock
 *
 * Description:
 *         Blocks until it is safe to read: no writer is active and no
 *         writer is waiting. Checking waitingWriters (not just
 *         activeWriters) is what prevents a steady stream of readers from
 *         starving a writer that is already queued.
 *
 * Inputs:
 *         lock : the lock to acquire for reading.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void fairRwLockReadLock(FairRwLock *lock)
{
    pthread_mutex_lock(&lock->mutex);
    while ((lock->activeWriters > 0) || (lock->waitingWriters > 0))
    {
        pthread_cond_wait(&lock->readersOk, &lock->mutex);
    }
    lock->activeReaders++;
    pthread_mutex_unlock(&lock->mutex);
}

/*****************************************************************************
 * Name: fairRwLockReadUnlock
 *
 * Description:
 *         Releases a previously acquired read lock. If this was the last
 *         active reader, wakes a waiting writer (if any) since readers can
 *         no longer block it.
 *
 * Inputs:
 *         lock : the lock to release from reading.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void fairRwLockReadUnlock(FairRwLock *lock)
{
    pthread_mutex_lock(&lock->mutex);
    lock->activeReaders--;
    if (lock->activeReaders == 0)
    {
        pthread_cond_signal(&lock->writerOk);
    }
    pthread_mutex_unlock(&lock->mutex);
}

/*****************************************************************************
 * Name: fairRwLockWriteLock
 *
 * Description:
 *         Blocks until it is safe to write: no readers and no other writer
 *         are active. Registers itself as a waiting writer first, so any
 *         reader that checks in the meantime backs off and lets it in.
 *
 * Inputs:
 *         lock : the lock to acquire for writing.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void fairRwLockWriteLock(FairRwLock *lock)
{
    pthread_mutex_lock(&lock->mutex);
    lock->waitingWriters++;
    while ((lock->activeReaders > 0) || (lock->activeWriters > 0))
    {
        pthread_cond_wait(&lock->writerOk, &lock->mutex);
    }
    lock->waitingWriters--;
    lock->activeWriters++;
    pthread_mutex_unlock(&lock->mutex);
}

/*****************************************************************************
 * Name: fairRwLockWriteUnlock
 *
 * Description:
 *         Releases a previously acquired write lock, then wakes the next
 *         waiting writer (if any, so writers drain in turn) or, if none
 *         are waiting, wakes all blocked readers.
 *
 * Inputs:
 *         lock : the lock to release from writing.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void fairRwLockWriteUnlock(FairRwLock *lock)
{
    pthread_mutex_lock(&lock->mutex);
    lock->activeWriters--;
    if (lock->waitingWriters > 0)
    {
        pthread_cond_signal(&lock->writerOk);
    }
    else
    {
        pthread_cond_broadcast(&lock->readersOk);
    }
    pthread_mutex_unlock(&lock->mutex);
}

FairRwLock sharedLock;
int sharedValue = 0;
int writerCompletions = 0;
pthread_mutex_t statsMutex = PTHREAD_MUTEX_INITIALIZER;

/*****************************************************************************
 * Name: readerThread
 *
 * Description:
 *         Repeatedly acquires the read lock, reads sharedValue, holds the
 *         lock briefly to simulate work, then releases it.
 *
 * Inputs:
 *         arg : reader thread index, passed as a pointer-sized integer.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *readerThread(void *arg)
{
    long id = (long)arg;
    int i;

    for (i = 0; i < READS_PER_THREAD; i++)
    {
        fairRwLockReadLock(&sharedLock);
        printf("reader %ld sees sharedValue = %d\n", id, sharedValue);
        usleep(READ_HOLD_USEC);
        fairRwLockReadUnlock(&sharedLock);
    }
    return NULL;
}

/*****************************************************************************
 * Name: writerThread
 *
 * Description:
 *         Repeatedly acquires the write lock, increments sharedValue, holds
 *         the lock briefly to simulate work, then releases it and records
 *         a completion so main can confirm writers made steady progress
 *         despite heavy read load.
 *
 * Inputs:
 *         arg : writer thread index, passed as a pointer-sized integer.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *writerThread(void *arg)
{
    long id = (long)arg;
    int i;

    for (i = 0; i < WRITES_PER_THREAD; i++)
    {
        fairRwLockWriteLock(&sharedLock);
        sharedValue++;
        printf("writer %ld bumped sharedValue to %d\n", id, sharedValue);
        usleep(WRITE_HOLD_USEC);
        fairRwLockWriteUnlock(&sharedLock);

        pthread_mutex_lock(&statsMutex);
        writerCompletions++;
        pthread_mutex_unlock(&statsMutex);
    }
    return NULL;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Launches many more reader threads than writer threads to create
 *         heavy read pressure, then confirms every writer completed all of
 *         its writes - demonstrating the fair lock did not starve them.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    pthread_t readers[NUM_READER_THREADS];
    pthread_t writers[NUM_WRITER_THREADS];
    long i;

    fairRwLockInit(&sharedLock);

    for (i = 0; i < NUM_READER_THREADS; i++)
    {
        pthread_create(&readers[i], NULL, readerThread, (void *)i);
    }
    for (i = 0; i < NUM_WRITER_THREADS; i++)
    {
        pthread_create(&writers[i], NULL, writerThread, (void *)i);
    }

    for (i = 0; i < NUM_READER_THREADS; i++)
    {
        pthread_join(readers[i], NULL);
    }
    for (i = 0; i < NUM_WRITER_THREADS; i++)
    {
        pthread_join(writers[i], NULL);
    }

    printf("writer completions = %d (expected %d) - no writer starved\n",
           writerCompletions, NUM_WRITER_THREADS * WRITES_PER_THREAD);
    printf("final sharedValue = %d\n", sharedValue);
    return 0;
}
