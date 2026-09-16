#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS_PER_THREAD 100000

int counter = 0;
pthread_mutex_t counterLock = PTHREAD_MUTEX_INITIALIZER;

void *incrementCounter(void *arg)
{
    int i;

    for (i = 0; i < INCREMENTS_PER_THREAD; i++)
    {
        // Without the lock, "counter++" is a read-modify-write that two
        // threads can interleave, silently dropping increments. The mutex
        // makes each increment atomic with respect to the other threads.
        pthread_mutex_lock(&counterLock);
        counter++;
        pthread_mutex_unlock(&counterLock);
    }

    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int i;
    int expected = NUM_THREADS * INCREMENTS_PER_THREAD;

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, incrementCounter, NULL);
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("counter = %d (expected %d)\n", counter, expected);

    pthread_mutex_destroy(&counterLock);
    return 0;
}
