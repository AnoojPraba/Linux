#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 5
#define ITEMS_TO_PRODUCE 10

int buffer[BUFFER_SIZE];
int count = 0;
int inIndex = 0;
int outIndex = 0;

pthread_mutex_t bufferLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;

// Unlike the single-slot producer/consumer in 49_Threads/03_conditionVariable.c,
// a bounded buffer holds up to BUFFER_SIZE items at once - the producer only
// blocks on notFull when the buffer is completely full, and the consumer
// only blocks on notEmpty when it's completely empty, so several items can
// be in flight without either side waiting on every single handoff.
void *producer(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < ITEMS_TO_PRODUCE; i++)
    {
        pthread_mutex_lock(&bufferLock);
        while (count == BUFFER_SIZE)
        {
            pthread_cond_wait(&notFull, &bufferLock);
        }

        buffer[inIndex] = i;
        inIndex = (inIndex + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&notEmpty);
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
        int item;

        pthread_mutex_lock(&bufferLock);
        while (count == 0)
        {
            pthread_cond_wait(&notEmpty, &bufferLock);
        }

        item = buffer[outIndex];
        outIndex = (outIndex + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&notFull);
        pthread_mutex_unlock(&bufferLock);

        printf("consumed: %d\n", item);
        consumed++;
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
    pthread_cond_destroy(&notFull);
    pthread_cond_destroy(&notEmpty);
    return 0;
}
