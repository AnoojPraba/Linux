#include <stdio.h>
#include <pthread.h>

#define ITEMS_TO_PRODUCE 5

int buffer = -1;
int itemReady = 0;
pthread_mutex_t bufferLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t itemReadyCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t itemConsumedCond = PTHREAD_COND_INITIALIZER;

void *producer(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < ITEMS_TO_PRODUCE; i++)
    {
        pthread_mutex_lock(&bufferLock);
        // This single-slot buffer only holds one item at a time, so the
        // producer must wait for the previous item to be consumed before
        // overwriting it - otherwise items are silently dropped and the
        // consumer's count never reaches ITEMS_TO_PRODUCE.
        while (itemReady)
        {
            pthread_cond_wait(&itemConsumedCond, &bufferLock);
        }
        buffer = i;
        itemReady = 1;
        // Wake the consumer up while still holding the lock - it will block
        // on the lock itself until we unlock, so this ordering is safe.
        pthread_cond_signal(&itemReadyCond);
        pthread_mutex_unlock(&bufferLock);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int consumed = 0;

    (void)arg;
    while (consumed < ITEMS_TO_PRODUCE)
    {
        pthread_mutex_lock(&bufferLock);
        // cond_wait releases the mutex while blocked and reacquires it
        // before returning - checking itemReady in a loop (not just "if")
        // guards against spurious wakeups.
        while (!itemReady)
        {
            pthread_cond_wait(&itemReadyCond, &bufferLock);
        }
        printf("consumed: %d\n", buffer);
        itemReady = 0;
        consumed++;
        pthread_cond_signal(&itemConsumedCond);
        pthread_mutex_unlock(&bufferLock);
    }
    return NULL;
}

int main()
{
    pthread_t producerThread;
    pthread_t consumerThread;

    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    pthread_mutex_destroy(&bufferLock);
    pthread_cond_destroy(&itemReadyCond);
    pthread_cond_destroy(&itemConsumedCond);
    return 0;
}
