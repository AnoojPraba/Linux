#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 4
#define INCREMENTS_PER_THREAD 100000

// An atomic_int guarantees each operation on it (here, the increment) runs
// as one indivisible step, without needing an explicit pthread_mutex_t -
// contrast with the mutex-protected counter in 09_Threads/02_mutex.c,
// which protects a plain int the same way but with an explicit lock/unlock.
atomic_int counter = 0;

void *incrementCounter(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < INCREMENTS_PER_THREAD; i++)
    {
        atomic_fetch_add(&counter, 1);
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

    printf("counter = %d (expected %d)\n", atomic_load(&counter), expected);

    return 0;
}
